#include "WebSocketServer.h"
#include "base64.h"
#include "sha-1.h"

#include <stddef.h>
#include <sstream>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ANSI_BLACK    "\033[0;30m"
#define ANSI_RED      "\033[0;31m"
#define ANSI_GREEN    "\033[0;32m"
#define ANSI_YELLOW   "\033[0;33m"
#define ANSI_BLUE     "\033[0;34m"
#define ANSI_MAGENTA  "\033[0;35m"
#define ANSI_CYAN     "\033[0;36m"
#define ANSI_GREY     "\033[0;37m"
#define ANSI_DARKGREY "\033[01;30m"
#define ANSI_BRED     "\033[01;31m"
#define ANSI_BGREEN   "\033[01;32m"
#define ANSI_BYELLOW  "\033[01;33m"
#define ANSI_BBLUE    "\033[01;34m"
#define ANSI_BMAGENTA "\033[01;35m"
#define ANSI_BCYAN    "\033[01;36m"
#define ANSI_WHITE    "\033[01;37m"
#define ANSI_NORMAL   "\033[0m"

#define COL_INFO ANSI_BLUE

//#define DEBUG_TRAFFIC
//#define FATAL_EXIT

static const char* GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

using namespace ws;

#ifdef DEBUG_TRAFFIC
static void printRaw(const uint8_t* data, int size) {
    for (int i=0; i<size; i++) {
        printf("%02x ", (uint8_t)data[i]);
    }
}
#endif

#ifdef FATAL_EXIT
static void fatal(const char* reason) {
    const char* msg = "";
    if (errno != 0) msg = strerror(errno);
    fprintf(stderr, "WebSocketServer: FATAL: %s: %s\n", reason, msg);
    exit(-1);
}
#else
#define fatal(x) assert(0 && x)
#endif

static std::string findKey(int size, const char* data) {
    // format: Sec-WebSocket-Key: 59Cn0ixeeVN+j5nQFCUlCw==
    static const char* id = "Sec-WebSocket-Key: ";
    const char* line = strstr(data, id);
    if (!line) return "";

    line += strlen(id);

    const char* cp = line;
    bool found = false;
    while (!found) {
        switch(*cp) {
        case 0:
        case ' ':
        case '\r':
        case '\n':
            found = true;
            break;
        }
        cp++;
    }
    cp--;
    std::string key(line, cp);
    return key;
}

enum Opcode {
    WS_TEXT     = 0x1,
    WS_BINARY   = 0x2,
    WS_CLOSE    = 0x8,
    WS_PING     = 0x9,
    WS_PONG     = 0xA,
};

static const char* opcode2str(uint8_t opcode) {
    switch (opcode) {
    case 0x0:       return "continuation";
    case WS_TEXT:   return "text";
    case WS_BINARY: return "binary";
    case 0x3:
    case 0x4:
    case 0x5:
    case 0x6:
    case 0x7:       return "non-control";
    case WS_CLOSE:  return "conn close";
    case WS_PING:   return "ping";
    case WS_PONG:   return "pong";
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:   return "reserved";
    }
    return "??";
}

WebSocketServer::WebSocketServer(int port_, WebSocketServer::Listener& listener_)
    : listener(listener_)
    , port(port_)
    , server_fd(-1)
    , random_fd(-1)
{
    random_fd = open("/dev/urandom", O_RDONLY);
    if (random_fd == -1) {
        fatal("unable to open /dev/urandom");
    }

    server_fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (server_fd == -1) {
        fatal("opening TCP socket");
    }

    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons((uint16_t)port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    int optval = 1;
    socklen_t optlen = sizeof(optval);
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) == -1) {
        fatal("setsockopt(SO_REUSEADDR) failed");
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) == -1) {
        fatal("setsockopt(SO_KEEPALIVE) failed");
    }
    if (setsockopt(server_fd, SOL_TCP, TCP_NODELAY, &optval, optlen) == -1) {
        fatal("setsockopt(TCP_NODELAY) failed");
    }

    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1) {
        fatal("fcntl(O_NONBLOCK)");
    }

    if (bind(server_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) != 0) {
        fatal("bind() failed");
    }
    if (::listen(server_fd, 4) != 0) {
        fatal("listen() failed");
    }
}

WebSocketServer::~WebSocketServer() {
    for (unsigned i=0; i<connections.size(); i++) {
        close(connections[i].conn_fd);
    }
    close(server_fd);
    close(random_fd);
}

void WebSocketServer::newConnection() {
    if (connections.size() == MAX_CONNECTIONS) {
        closeConnection(0, true);
    }
    char* src_ip = 0;
    int conn_fd = accept(&src_ip);
    if (conn_fd == -1) {
        perror("accept");
        return;
    }
    connections.push_back(Connection(conn_fd));
    listener.onNewConnection(src_ip);
}

void WebSocketServer::closeConnection(unsigned index, bool self) {
    // NOTE: dont remove connections here to avoid modifications during iteration
    if (self) printf(COL_INFO "closed connection (%d)"ANSI_NORMAL"\n", index);
    else printf(COL_INFO "client closed connection (%d)"ANSI_NORMAL"\n", index);
    close(connections[index].conn_fd);
    connections[index].conn_fd = -1;
    listener.onLostConnection();
}

void WebSocketServer::handleData(unsigned index) {
    static const int MAX_RECEIVE = 8192;
    char buffer[MAX_RECEIVE];
    int fd = connections[index].conn_fd;
    ssize_t recv_bytes = read(fd, &buffer, MAX_RECEIVE);
    if (recv_bytes == -1 || recv_bytes == 0) {
        closeConnection(index, false);
        return;
    }
    buffer[recv_bytes] = 0; // just null-terminate for easy printing
#ifdef DEBUG_TRAFFIC
    printf("---- [RECV %d:%d bytes] ----\n", index, recv_bytes);
#endif
    switch (connections[index].state) {
    case WS_HANDSHAKE:
#ifdef DEBUG_TRAFFIC
        printf(ANSI_GREEN "%s"ANSI_NORMAL"\n", buffer);
#endif
        handle_handshake(index, recv_bytes, buffer);
        break;
    case WS_DATA_TRANSFER:
        handle_data(index, recv_bytes, (uint8_t*)buffer);
        break;
    }
}

void WebSocketServer::step() {
    fd_set rdfs;
    FD_ZERO(&rdfs);
    FD_SET(server_fd, &rdfs);
    int max_fd = server_fd;

    for (unsigned i=0; i<connections.size(); i++) {
        int fd = connections[i].conn_fd;
        FD_SET(fd, &rdfs);
        if (fd > max_fd) max_fd = fd;
    }

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    int res = select(max_fd + 1, &rdfs, NULL, NULL, &timeout);
    if (res < 0) {
        if (errno == EINTR) return;
        else {
            const char* msg = strerror(errno);
            fprintf(stderr, "error on select: %s\n", msg);
            return;
        }
    }
    if (res > 0) {
        for (unsigned i=0; i<connections.size(); i++) {
            if (FD_ISSET(connections[i].conn_fd, &rdfs)) {
                handleData(i);
            }
        }
        if (FD_ISSET(server_fd, &rdfs)) {
            newConnection();
        }
        cleanupConnections();
    }
}

void WebSocketServer::cleanupConnections() {
    ConnectionsIter it = connections.begin();
    while (it != connections.end()) {
        if ((*it).conn_fd == -1) {
            it = connections.erase(it);
        } else {
            ++it;
        }
    }
}

void WebSocketServer::send(unsigned index, int size, const uint8_t* data) {
#ifdef DEBUG_TRAFFIC
    // parse self-generated header
    printf("---- [SEND %d bytes] ----\n", size);
    {
        bool fin = data[0] & 0x80;
        uint8_t opcode = data[0] & 0x0F;
        bool haveMask = data[1] & 0x80;
        uint32_t payload = data[1] & 0x7F;
        if (payload == 126) {
            payload = (data[2] << 8) | data[3];
        } else if (payload == 127) {
            assert(0 && "BIG frames not supported");
        }
        printf("MSG: fin=%d, opcode=0x%x(%s) mask=%d, payload=%d\n", fin, opcode, opcode2str(opcode), haveMask, payload);
    }
    switch (connections[index].state) {
    case WS_HANDSHAKE:
        printf(ANSI_RED "%s"ANSI_NORMAL"\n", data);
        break;
    case WS_DATA_TRANSFER:
        printf("  "ANSI_RED);
        printRaw((const uint8_t*)data, size);
        printf(ANSI_NORMAL"\n");
        break;
    }
#endif
    ssize_t written = write(connections[index].conn_fd, data, size);
    if (written != size) {
        // TODO log, not fatal
        fatal("sending");
    }
}

static void maskData(uint8_t* out, const uint8_t* in, uint32_t size, const uint8_t* mask) {
    for (uint32_t i=0; i<size; i++) {
        out[i] = in[i] ^ mask[i%4];
    }
}

void WebSocketServer::sendData(uint32_t size, const uint8_t* data) {
    // RFC 6455 5.1: A server MUST NOT mask any frames it sends to a client
    uint8_t frame[size + 10];
    frame[0] = 0x81;    // FIN, text frame
    int header = 1;
    if (size <= 125) {
        frame[1] = (0x00 | size);   // NO mask + size<126
        header += 1;
    } else if (size <= 65535) {
        frame[1] = (0x00 | 126);
        frame[2] = ((size >> 8) & 0xFF);
        frame[3] = (size & 0xFF);
        header += 3;
    } else {
        assert(0 && "TODO large size >65535 not supported!");
    }
    memcpy(&frame[header], data, size);
    int len = header + size;
    for (unsigned i=0; i<connections.size(); i++) {
        if (connections[i].ready()) send(i, len, frame);
    }

}

void WebSocketServer::handle_handshake(unsigned index, int size, char* data) {
    std::stringstream ss;
    ss << "HTTP/1.1 101 Switching Protocols\x0d\x0a";
    ss << "Upgrade: WebSocket\x0d\x0a";
    ss << "Connection: Upgrade\x0d\x0a";
    std::string input = findKey(size, data);
    if (input == "") {
        printf(COL_INFO "cannot find key, closing connection"ANSI_NORMAL"\n");
        closeConnection(index, true);
        return;
    }
    char result[256];
    int len = sprintf(result, "%s%s", input.c_str(), GUID);

    uint8_t hash[20];
    SHA1((uint8_t*)result, len, hash);

    std::string final = base64_encode(hash, 20);
    ss << "Sec-WebSocket-Accept: " << final << "\x0d\x0a";
    ss << "\x0d\x0a";

    const std::string s = ss.str();
    connections[index].state = WS_DATA_TRANSFER;
    send(index, s.size(), (const uint8_t*)s.c_str());
}

void WebSocketServer::handle_data(unsigned index, int size, const uint8_t* data) {
    assert(size >= 3);
#ifdef DEBUG_TRAFFIC
    printf("  "ANSI_GREEN);
    printRaw(data, size);
    printf(ANSI_NORMAL"\n");
#endif
    bool fin = data[0] & 0x80;
    uint8_t opcode = data[0] & 0x0F;
    bool haveMask = data[1] & 0x80;
    uint32_t payload = data[1] & 0x7F;
    const uint8_t* cp = data + 2;
    if (payload == 126) {
        payload = (data[2] << 8) | data[3];
        cp += 2;
    } else if (payload == 127) {
        assert(0 && "BIG frames not supported");
        return;
    }
#ifdef DEBUG_TRAFFIC
    printf("MSG: fin=%d, opcode=0x%x(%s) mask=%d, payload=%d\n", fin, opcode, opcode2str(opcode), haveMask, payload);
#endif
    assert(fin && "fragmentation is not supported");
    assert(haveMask);
    uint8_t mask[4];
    if (haveMask) {
        mask[0] = cp[0];
        mask[1] = cp[1];
        mask[2] = cp[2];
        mask[3] = cp[3];
        cp += 4;
    }
    // cp points to data
    ptrdiff_t header_len = cp -(uint8_t*)data;
    if (size != (int)(payload + header_len)) {
        printf("warning: invalid data size: expected %d bytes, got %d\n", header_len + payload, size);
        return;
    }
    // unmask data
    uint8_t unmask[payload+1];  // +1 for 0-termination (only used for text)
    maskData(unmask, cp, payload, mask);
    unmask[payload] = 0;

    switch (opcode) {
    case WS_TEXT:
#ifdef DEBUG_TRAFFIC
        printf("  text: '%s'\n", unmask);
#endif
        listener.onReceiveData(payload, unmask);
        break;
    case WS_BINARY:
        listener.onReceiveData(payload, unmask);
        break;
    case WS_CLOSE:
#ifdef DEBUG_TRAFFIC
        if (payload) {
            assert(payload >= 2);
            uint32_t reason = (unmask[0] << 8) | unmask[1];
            printf("  close: reason=%d", reason);
            // NOTE: for value semantics, see:  https://tools.ietf.org/html/rfc6455#section-7.4
            printf("  text='%s'", unmask+2);
            printf("\n");
        }
#endif
        closeConnection(index, false);
        break;
    case WS_PING:
        assert(0 && "TODO ping");
        break;
    case WS_PONG:
        assert(0 && "TODO pong");
        break;
    default:
        printf("warning: opcode 0x%x(%s) not supported, ignoring\n", opcode, opcode2str(opcode));
        return;
    }
}

void WebSocketServer::readRandomMask(uint8_t* mask) {
    ssize_t numread = read(random_fd, mask, 4);
    if (numread != 4) {
        fatal("cannot read random data");
    }
}

int WebSocketServer::accept(char** src_ipnr) {
    uint32_t sin_size = sizeof(struct sockaddr_in);
    struct sockaddr_in remote;
    int new_fd = ::accept4(server_fd, (struct sockaddr*)&remote, &sin_size, SOCK_CLOEXEC);
    if (new_fd == -1) {
        perror("accept4");
        return -1;
    }
    *src_ipnr = inet_ntoa(remote.sin_addr);
    return new_fd;
}

