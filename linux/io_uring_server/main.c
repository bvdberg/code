#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <liburing.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#include "logger.h"

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

struct io_uring ring;

static TimeoutRequest timer_request;
//static TimeoutRequest timer_request2;
static AcceptRequest accept_request;
static ReadRequest read_request; // for stdin


static bool on_write(void* arg, int res);

// TODO use pool for get_write/put_write
static WriteRequest* get_write(void) {
    WriteRequest* req = malloc(sizeof(WriteRequest));
    req->iov.iov_base = malloc(WRITE_SZ);
    req->iov.iov_len = WRITE_SZ;
    req->req.handler = on_write;
    return req;
}

static void put_write(WriteRequest* req) {
    free(req->iov.iov_base);
    free(req);
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

static void add_timeout_request(TimeoutRequest* req) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    unsigned count = 0;
    unsigned flags = 0;
    uint64_t current_time = now();

    assert(req->interval_end > current_time);
    uint64_t delay = req->interval_end - current_time;
    //log_info("delay %ld", delay);
    req->ts.tv_sec = 0;
    req->ts.tv_nsec = delay * 1000;
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
        log_warn("accept request failed: %s", strerror(-res));
        exit(EXIT_FAILURE);
    }
    int len = res;

    ReadRequest* req = arg;
    char* cp = req->iov.iov_base;
    // TEMP strip NL
    if (len > 1 && cp[len-1] == '\n') cp[len-1] = 0;
    printf("READ STDIN %d/%ld [%s]\n", len, req->iov.iov_len, cp);
    add_read_request(req);
    return true;
}

static bool on_write(void* arg, int res) {
    if (res <= 0) {
        log_warn("write failed: %s", strerror(-res));
        // TODO close connection, etc
        exit(EXIT_FAILURE);
    }
    WriteRequest* req = arg;
    //log_info("write[%d] done", req->fd);
    put_write(req);
    return false;
}

static bool on_close(void* arg, int res) {
    ReadRequest* req = arg;
    log_info("closed[%d] %d", req->fd, res);
    free(req);
    return false;
}

static bool on_timeout(void* arg, int len) {
    TimeoutRequest* req = arg;
    log_info("timeout %s", req->name);
    req->interval_end += req->interval;
    add_timeout_request(req);
    return true;
}

static bool on_read_socket(void* arg, int res) {
    ReadRequest* req = arg;
    if (res <= 0) {
        // TODO close socket
        log_info("socket %d was closed", req->fd);
        req->req.handler = on_close; // change handler
        add_close_request(req);
        //log_warn("accept request failed: (%d) %s", res, strerror(-res));
        return true;
    }

    char* input = req->iov.iov_base;
    int len = res;
    // TEMP strip CR/NL
    if (len > 2 && input[len-2] == '\r') input[len-2] = 0;
    printf("READ SOCKET[%d] %d/%ld [%s]\n", req->fd, len, req->iov.iov_len, input);

    add_read_request(req);
    WriteRequest* wr = get_write();
    wr->fd = req->fd;
    char* output = wr->iov.iov_base;
    wr->iov.iov_len = sprintf(output, "ECHO %s\n", input);

    // TODO change handler? (to close socket, cancel read, etc)
    add_write_request(wr);
    return true;
}

static bool on_accept(void* arg, int res) {
    AcceptRequest* req = arg;
    if (res <= 0) {
        log_warn("accept request failed: %s", strerror(-res));
        exit(EXIT_FAILURE);
    }
    add_accept_request(req);

    log_info("accept (res %d)", res);
    ReadRequest* r2 = malloc(sizeof(ReadRequest));
    r2->req.handler = on_read_socket;
    r2->fd = res;
    r2->iov.iov_base = malloc(READ_SZ);
    r2->iov.iov_len = READ_SZ;
    add_read_request(r2);
    return true;
}

void server_loop(void) {
    struct io_uring_cqe *cqe;

    while (1) {
        int ret = io_uring_wait_cqe(&ring, &cqe);
        if (ret < 0) fatal_error("io_uring_wait_cqe");
        int submissions = 0;

        while (1) {
            // TODO use multiple loops with peek (see RedHat blog)
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

void sigint_handler(int signo) {
    log_info("^C pressed. Shutting down.");
    io_uring_queue_exit(&ring);
    exit(0);
}

int main() {
    log_init(Info, true, true);
    int server_socket = setup_listening_socket(DEFAULT_SERVER_PORT);

    uint64_t interval = 1000000;
    timer_request.name = "T1";
    timer_request.req.handler = on_timeout;
    timer_request.interval_end = now() + interval;
    timer_request.interval = interval;

#if 0
    interval = 250000;
    timer_request2.name = "T2";
    timer_request2.req.handler = on_timeout;
    timer_request2.interval_end = now() + interval;
    timer_request2.interval = interval;
#endif

    accept_request.req.handler = on_accept;
    accept_request.fd = server_socket;
    accept_request.client_addr_len = sizeof(accept_request.client_addr);

    read_request.req.handler = on_read_stdin;
    read_request.fd = 1;
    read_request.iov.iov_base = malloc(READ_SZ);
    read_request.iov.iov_len = READ_SZ;

    signal(SIGINT, sigint_handler);
    io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
    log_info("listening on port %d", DEFAULT_SERVER_PORT);

    add_timeout_request(&timer_request);
    //add_timeout_request(&timer_request2);
    add_read_request(&read_request);
    add_accept_request(&accept_request);
    io_uring_submit(&ring);

    server_loop();

    return 0;
}

