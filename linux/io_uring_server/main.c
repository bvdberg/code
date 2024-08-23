#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <liburing.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "logger.h"

#define DEFAULT_SERVER_PORT     8000
#define QUEUE_DEPTH             256
#define READ_SZ                 8192

#define EVENT_TYPE_ACCEPT       0
#define EVENT_TYPE_READ         1
#define EVENT_TYPE_WRITE        2
#define EVENT_TYPE_CLOSE        3
#define EVENT_TYPE_TIMEOUT      4

typedef struct {
    int event_type;
    int iovec_count;
    int client_socket;
    struct iovec iov[];
} Request;

struct io_uring ring;

void fatal_error(const char *syscall) {
    perror(syscall);
    exit(1);
}

void *zh_malloc(size_t size) {
    void *buf = malloc(size);
    if (!buf) {
        fprintf(stderr, "Fatal error: unable to allocate memory.\n");
        exit(1);
    }
    return buf;
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



static void add_timeout_request(void) {
    log_info("add timeout");
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    unsigned count = 1;
    unsigned flags = 0;
    // note: ts must remain alive until submitted!
    struct __kernel_timespec ts = { .tv_sec = 1, .tv_nsec = 0 };

    Request *req = malloc(sizeof(*req));
    req->event_type = EVENT_TYPE_TIMEOUT;
    //flags = IORING_TIMEOUT_MULTISHOT;
    io_uring_prep_timeout(sqe, &ts, count, flags);
    io_uring_sqe_set_data(sqe, req);
    io_uring_submit(&ring);
}

static void add_accept_request(int server_socket, struct sockaddr_in *client_addr, socklen_t *client_addr_len) {
    log_info("add accept");
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_accept(sqe, server_socket, (struct sockaddr *) client_addr,
                         client_addr_len, 0);
    Request *req = malloc(sizeof(*req));
    req->event_type = EVENT_TYPE_ACCEPT;
    io_uring_sqe_set_data(sqe, req);
    io_uring_submit(&ring);
}

static void add_read_request(int client_socket) {
    log_info("add read");
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    // TODO get from pool
    Request *req = malloc(sizeof(*req) + sizeof(struct iovec));
    req->iov[0].iov_base = malloc(READ_SZ);
    req->iov[0].iov_len = READ_SZ;
    req->event_type = EVENT_TYPE_READ;
    req->client_socket = client_socket;
    memset(req->iov[0].iov_base, 0, READ_SZ);
    /* Linux kernel 5.5 has support for readv, but not for recv() or read() */
    io_uring_prep_readv(sqe, client_socket, &req->iov[0], 1, 0);
    io_uring_sqe_set_data(sqe, req);
    io_uring_submit(&ring);
}

static void add_write_request(Request *req) {
    log_info("add write");
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    req->event_type = EVENT_TYPE_WRITE;
    io_uring_prep_writev(sqe, req->client_socket, req->iov, req->iovec_count, 0);
    io_uring_sqe_set_data(sqe, req);
    io_uring_submit(&ring);
}

static void add_close_request(Request *req) {
    log_info("add close");
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    req->event_type = EVENT_TYPE_CLOSE;
    io_uring_prep_close(sqe, req->client_socket);
    io_uring_submit(&ring);
}

static void send_static_string_content(const char *str, int client_socket) {
    Request *req = zh_malloc(sizeof(*req) + sizeof(struct iovec));
    unsigned long slen = strlen(str);
    req->iovec_count = 1;
    req->client_socket = client_socket;
    req->iov[0].iov_base = zh_malloc(slen);
    req->iov[0].iov_len = slen;
    memcpy(req->iov[0].iov_base, str, slen);
    add_write_request(req);
}

static void dump(const char* data, size_t len) {
    char buf[1024];
    char* cp = buf;
    for (int i = 0; i < 128; i++) {
        if (data[i] == 0) break;
        cp += sprintf(cp, "%02x ", data[i]);
    }
    printf("INCOMING %ld [%s]\n", len, buf);
}

static void handle_incoming(Request *req, int server_socket) {
    /* Get the first line, which will be the request */
    char* data = (char*)req->iov[0].iov_base;
    size_t len = req->iov[0].iov_len;
    dump(data, len);
    int slen = strlen(data);
    if (data[slen-2] == '\r') data[slen-2] = 0;
    // slen not valid then

    // no overflow checking
    char buf[128];
    sprintf(buf, "ECHO '%s'\n", data);
    send_static_string_content(buf, req->client_socket);

    if (strcmp(data, "quit") == 0) {
        add_close_request(req);
    } else {
        add_read_request(req->client_socket);
    }
}

void server_loop(int server_socket) {
    struct io_uring_cqe *cqe;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    add_accept_request(server_socket, &client_addr, &client_addr_len);

    while (1) {
        int ret = io_uring_wait_cqe(&ring, &cqe);
        if (ret < 0) fatal_error("io_uring_wait_cqe");

        Request *req = (Request*) cqe->user_data;
        if (cqe->res < 0) {
            log_warn("Async request failed: %s for event: %d",
                    strerror(-cqe->res), req->event_type);
            exit(1);
        }

        // TODO fix malloc/freeing of Requests all the time
        switch (req->event_type) {
            case EVENT_TYPE_ACCEPT:
                log_info("complete: accept");
                add_accept_request(server_socket, &client_addr, &client_addr_len);
                add_read_request(cqe->res);
                add_timeout_request(); // wil also submit
                free(req);
                break;
            case EVENT_TYPE_READ:
                log_info("complete: read");
                if (cqe->res <= 0) {
                    log_warn("Empty request! (connection closed)"); // connection closed
                    // TODO also handle via
                    add_close_request(req);
                    //free(req->iov[0].iov_base);
                    //free(req);
                } else {
                    handle_incoming(req, server_socket);
                    //free(req->iov[0].iov_base);
                    //free(req);
                }
                break;
            case EVENT_TYPE_WRITE:
                log_info("complete: write");
                for (int i = 0; i < req->iovec_count; i++) {
                    free(req->iov[i].iov_base);
                }
                //close(req->client_socket);
                free(req);
                break;
            case EVENT_TYPE_CLOSE:
                log_info("complete: close");
                free(req);
                break;
            case EVENT_TYPE_TIMEOUT:
                log_info("complet: timeout");
                break;
            default:
                log_warn("Unexpected req type %d", req->event_type);
                break;
        }
        /* Mark this request as processed */
        io_uring_cqe_seen(&ring, cqe);
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

    signal(SIGINT, sigint_handler);
    io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
    log_info("listening on port %d", DEFAULT_SERVER_PORT);
    server_loop(server_socket);

    return 0;
}
