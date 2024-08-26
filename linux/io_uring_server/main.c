#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/signalfd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <liburing.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#include "logger.h"
#include "linked_list.h"

#define DEFAULT_SERVER_PORT     8000
#define QUEUE_DEPTH             256
#define READ_SZ                 4096
#define WRITE_SZ                4096
#define MAX_SQE_PER_LOOP 10

static uint64_t now() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_nsec / 1000);
    return now64;
}

typedef bool (*HandlerFn)(void* r, int len);

typedef struct {
    HandlerFn handler;
} Request;

typedef struct {
    Request req;    // must be first
    const char* name; // no ownership
    struct __kernel_timespec ts;
    uint64_t interval_end;
    uint64_t interval;
} TimeoutRequest;

typedef struct {
    Request req;    // must be first
    int fd;
    struct iovec iov;
} ReadRequest;

typedef struct {
    Request req;    // must be first
    int fd;
    struct iovec iov;
} WriteRequest;

typedef struct {
    Request req;    // must be first
    int fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
} AcceptRequest;

typedef struct {
    ReadRequest read_request; // must be first member (we dont use to_container yet)
    struct list_tag list;
    uint64_t count;
} Connection;

struct io_uring ring;

static TimeoutRequest timer_request;
static TimeoutRequest timer_request2;
static AcceptRequest accept_request;
static ReadRequest read_stdin;
static ReadRequest read_signal;
static bool stop;
static int client_fd = -1;
static uint64_t tx_count;
static uint64_t last_count;
static struct list_tag conns;

static bool on_write(void* arg, int res);
static void removeConnection(Connection* c);
static void dumpConnections(void);

// Pool
#define WR_POOL_SIZE 32
static int wr_count;
static int wr_capacity;
static WriteRequest* wr_pool[WR_POOL_SIZE];

static void wr_pool_init(void) {
    wr_count = WR_POOL_SIZE;
    wr_capacity = WR_POOL_SIZE;
    for (int i = 0; i < WR_POOL_SIZE; i++) {
        WriteRequest* req = malloc(sizeof(WriteRequest));
        req->iov.iov_base = malloc(WRITE_SZ);
        req->iov.iov_len = WRITE_SZ;
        req->req.handler = on_write;
        wr_pool[i] = req;
    }
}

static void wr_pool_free(void) {
    for (int i = 0; i < WR_POOL_SIZE; i++) {
        free(wr_pool[i]->iov.iov_base);
        free(wr_pool[i]);
    }
}

static WriteRequest* get_write(void) {
    assert(wr_count);
    wr_count--;
    return wr_pool[wr_count];
}

static void put_write(WriteRequest* req) {
    assert(wr_count != wr_capacity);
    wr_pool[wr_count] = req;
    wr_count++;
}

void fatal_error(const char *syscall) {
    perror(syscall);
    exit(1);
}

static int setup_listening_socket(int port) {
    int sock;
    struct sockaddr_in srv_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        fatal_error("socket()");

    int enable = 1;
    if (setsockopt(sock,
                   SOL_SOCKET, SO_REUSEADDR,
                   &enable, sizeof(int)) < 0)
        fatal_error("setsockopt(SO_REUSEADDR)");


    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (const struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0)
        fatal_error("bind()");

    if (listen(sock, 10) < 0)
        fatal_error("listen()");

    return sock;
}

static int setup_signalfd(void) {
    sigset_t mask;
    //struct signalfd_siginfo fdsi;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGPIPE);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        fatal_error("sigprocmask");
    }

    int sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) fatal_error("signalfd");
    return sfd;
}

static void add_timeout_request(TimeoutRequest* req) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    unsigned count = 0;
    unsigned flags = 0;
    uint64_t current_time = now();

    // TEMP
#if 1
    assert(req->interval_end > current_time);
    uint64_t delay = req->interval_end - current_time;
    //log_info("delay %ld", delay);
    req->ts.tv_sec = 0;
    req->ts.tv_nsec = delay * 1000;
#else
    req->ts.tv_sec = 0;
    req->ts.tv_nsec = 1000;
#endif
    io_uring_prep_timeout(sqe, &req->ts, count, flags);
    io_uring_sqe_set_data(sqe, req);
}

static void add_accept_request(AcceptRequest* req) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_accept(sqe, req->fd, (struct sockaddr *) &req->client_addr, &req->client_addr_len, 0);
    io_uring_sqe_set_data(sqe, req);
}

static void add_read_request(ReadRequest* req) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_readv(sqe, req->fd, &req->iov, 1, 0);
    io_uring_sqe_set_data(sqe, req);
}

static void add_write_request(WriteRequest *req) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_writev(sqe, req->fd, &req->iov, 1, 0);
    io_uring_sqe_set_data(sqe, req);
}

static void add_close_request(ReadRequest *req) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_close(sqe, req->fd);
    io_uring_sqe_set_data(sqe, req);
}

static bool on_read_stdin(void* arg, int res) {
    if (res <= 0) {
        log_warn("read request failed: %s", strerror(-res));
        exit(EXIT_FAILURE);
    }
    int len = res;

    ReadRequest* req = arg;
    char* cp = req->iov.iov_base;
    // TEMP strip NL
    if (len > 1 && cp[len-1] == '\n') cp[len-1] = 0;
    //printf("READ STDIN %d/%ld [%s]\n", len, req->iov.iov_len, cp);
    dumpConnections();
    add_read_request(req);
    return true;
}

static bool on_read_signals(void* arg, int res) {
    if (res <= 0) {
        log_warn("read request failed: %s", strerror(-res));
        exit(EXIT_FAILURE);
    }
    ReadRequest* req = arg;
    if (res != sizeof(struct signalfd_siginfo)) {
        log_warn("invalid size");
        exit(EXIT_FAILURE);
    }
    const struct signalfd_siginfo* fdsi = req->iov.iov_base;
    if (fdsi->ssi_signo == SIGINT) {
        log_info("Got SIGINT");
    } else if (fdsi->ssi_signo == SIGQUIT) {
        log_info("Got SIGQUIT");
    } else if (fdsi->ssi_signo == SIGPIPE) {
        log_info("Got SIGPIPE");
    } else {
        log_warn("Read unexpected signal %d\n", fdsi->ssi_signo);
    }

    stop = true;
    //add_read_request(req);
    return false;
}

static bool on_write(void* arg, int res) {
    // Note: when write fails, read will also fail, close on read (since we need a ReadRequest
    WriteRequest* req = arg;
    if (res <= 0) {
        log_info("write[%d] socket was closed (%s)", req->fd, strerror(-res));
        client_fd = -1;
        // wait for read to fail
    }
    put_write(req);
    return false;
}

static bool on_close(void* arg, int res) {
    ReadRequest* req = arg;
    log_info("closed[%d] %d", req->fd, res);
    removeConnection((Connection*)req);
    return false;
}

static bool on_timeout(void* arg, int len) {
    TimeoutRequest* req = arg;
    log_info("tick (rx/tx %ld/%ld) (+%ld)", tx_count, tx_count, tx_count - last_count);
    last_count = tx_count;
    req->interval_end += req->interval;
    add_timeout_request(req);
    return true;
}

static bool on_timeout2(void* arg, int len) {
    TimeoutRequest* req = arg;
    req->interval_end += req->interval;
    add_timeout_request(req);

    if (client_fd != -1) {
        //log_info("send");
        WriteRequest* wr = get_write();
        wr->fd = client_fd;
        char* output = wr->iov.iov_base;
        wr->iov.iov_len = 4096;
        add_write_request(wr);
    }
    return true;
}

static bool on_read_socket(void* arg, int res) {
    ReadRequest* req = arg;
    if (res <= 0) {
        log_info("read[%d] socket was closed (%s)", req->fd, strerror(-res));
        req->req.handler = on_close; // change handler
        add_close_request(req);
        client_fd = -1;
        //log_warn("read request failed: (%d) %s", res, strerror(-res));
        return true;
    }

    char* input = req->iov.iov_base;
    int len = res;
#if 0
    // TEMP strip CR/NL
    if (len > 2 && input[len-2] == '\r') {
        input[len-2] = 0;
        len -= 2;
    }
#endif
    //printf("READ SOCKET[%d] %d/%ld [%s]\n", req->fd, len, req->iov.iov_len, input);

    add_read_request(req);
#if 1
    // echo back
    WriteRequest* wr = get_write();
    wr->fd = req->fd;
    char* output = wr->iov.iov_base;
    memcpy(output, input, len);
    wr->iov.iov_len = len;

    Connection* c = arg;
    c->count++;
    tx_count++;
    add_write_request(wr);
#endif
    return true;
}

static Connection* createConnection(int fd) {
    Connection* conn = malloc(sizeof(Connection));
    ReadRequest* r2 = &conn->read_request;
    r2->req.handler = on_read_socket;
    r2->fd = fd;
    r2->iov.iov_base = malloc(READ_SZ);
    r2->iov.iov_len = READ_SZ;
    add_read_request(r2);
    return conn;
}

static void removeConnection(Connection* c) {
    list_remove(&c->list);
    free(c->read_request.iov.iov_base);
    free(c);
}

static void dumpConnections(void) {
    list_t cur = conns.next;
    uint32_t idx = 0;
    while (cur != &conns) {
        Connection* c = to_container(cur, Connection, list);
        printf("[%2d] %2d  %ld\n", idx, c->read_request.fd, c->count);
        idx++;
        cur = cur->next;
    }
}

static bool on_accept(void* arg, int res) {
    AcceptRequest* req = arg;
    if (res <= 0) {
        log_warn("accept request failed: %s", strerror(-res));
        exit(EXIT_FAILURE);
    }
    add_accept_request(req);

    log_info("accept (res %d) from %s", res, inet_ntoa(req->client_addr.sin_addr));

    Connection* c = createConnection(res);
    list_add_tail(&conns, &c->list);

    client_fd = res;
    return true;
}

void server_loop(void) {
    struct io_uring_cqe *cqe;

    while (!stop) {
        int ret = io_uring_wait_cqe(&ring, &cqe);
        if (ret < 0) fatal_error("io_uring_wait_cqe");
        int submissions = 0;

        // TODO try io_uring_prep_read_fixed + io_uring_register_buffers (Fixed buffers)
        // TODO try io_uring_for_each_cqe() + io_uring_cq_advance(ring, num)
        // TODO try kernel worker thread (need newer kernel?)

        while (!stop) {
            Request *req = (Request*) cqe->user_data;
            assert(req);
            submissions += req->handler(req, cqe->res);
            // dont use req anymore here, might be freed
            io_uring_cqe_seen(&ring, cqe);
            if (io_uring_sq_space_left(&ring) < MAX_SQE_PER_LOOP) break;

            ret = io_uring_peek_cqe(&ring, &cqe);
            if (ret < 0) {
                if (ret == -EAGAIN) break;     // no remaining work in completion queue
                log_error("peek failed: (%d) %s", ret, strerror(-ret));
                exit(EXIT_FAILURE);
            }
        }
        if (submissions != 0) {
            io_uring_submit(&ring);
        }
    }
}

#if 0
void sigint_handler(int signo) {
    log_info("^C pressed. Shutting down.");
    io_uring_queue_exit(&ring);
    exit(0);
}
#endif

static int cfg_cpu = 3;
static void set_cpu_affinity(void) {
    cpu_set_t mask;

    if (cfg_cpu == -1)
        return;

    CPU_ZERO(&mask);
    CPU_SET(cfg_cpu, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask))
        fatal_error("unable to pin cpu");
}

static void set_iowq_affinity(struct io_uring *r) {
    cpu_set_t mask;
    int ret;

    if (cfg_cpu == -1)
        return;

    CPU_ZERO(&mask);
    CPU_SET(cfg_cpu, &mask);
    ret = io_uring_register_iowq_aff(r, 1, &mask);
    if (ret)
        fatal_error("unabled to set io-wq affinity");
}


int main() {
    log_init(Info, true, true);

    list_init(&conns);
    wr_pool_init();

    int server_socket = setup_listening_socket(DEFAULT_SERVER_PORT);
    log_info("listening on port %d", DEFAULT_SERVER_PORT);

    int sfd = setup_signalfd();

    io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
    //set_cpu_affinity();
    //set_iowq_affinity(&ring);

    uint64_t interval = 1000000;
    timer_request.req.handler = on_timeout;
    timer_request.interval_end = now() + interval;
    timer_request.interval = interval;

#if 1
    interval = 2000;
    timer_request2.req.handler = on_timeout2;
    timer_request2.interval_end = now() + interval;
    timer_request2.interval = interval;
#endif

    accept_request.req.handler = on_accept;
    accept_request.fd = server_socket;
    accept_request.client_addr_len = sizeof(accept_request.client_addr);

    read_signal.req.handler = on_read_signals;
    read_signal.fd = sfd;
    read_signal.iov.iov_base = malloc(READ_SZ);
    read_signal.iov.iov_len = READ_SZ;

    read_stdin.req.handler = on_read_stdin;
    read_stdin.fd = 1;
    read_stdin.iov.iov_base = malloc(READ_SZ);
    read_stdin.iov.iov_len = READ_SZ;

    add_timeout_request(&timer_request);
    //add_timeout_request(&timer_request2);
    add_read_request(&read_signal);
    add_read_request(&read_stdin);
    add_accept_request(&accept_request);
    io_uring_submit(&ring);

    log_info("press ENTER to show connections");

    server_loop();

    log_info("done");

    wr_pool_free();
    // TODO free mallocs above

    list_t cur = conns.next;
    while (cur != &conns) {
        Connection* c = to_container(cur, Connection, list);
        cur = cur->next;
        list_remove(&c->list);
    }

    // TODO use wait_cqe_fast(&ring); to wait for all events
    io_uring_queue_exit(&ring);

    return 0;
}

