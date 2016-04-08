#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <stdint.h>
#include <string>
#include <deque>

namespace ws {

class WebSocketServer {
public:
    static const unsigned MAX_CONNECTIONS = 4;

    class Listener {
    public:
        virtual ~Listener() {}

        virtual void onNewConnection(const char* src_ip) = 0;
        virtual void onLostConnection() = 0;
        virtual void onReceiveData(uint32_t size, const uint8_t* data) = 0;
    };

    WebSocketServer(int port_, Listener& listener_);
    ~WebSocketServer();

    // when using external event mechanism (select, poll, etc)
    void newConnection();
    void handleData(unsigned index);
    int getServerFd() const { return server_fd; }
    int getNumConnections() const { return connections.size(); }
    int getConnFd(unsigned i) const { return connections[i].conn_fd; }

    // when using periodic step() function
    void step();

    void sendData(uint32_t size, const uint8_t* data);
private:
    void closeConnection(unsigned index, bool self);
    void cleanupConnections();
    void send(unsigned index, int size, const uint8_t* data);
    void handle_handshake(unsigned index, int size, char* data);
    void handle_data(unsigned index, int size, const uint8_t* data);
    void readRandomMask(uint8_t* mask);
    int accept(char** src_ipnr);

    enum State {
        WS_HANDSHAKE,
        WS_DATA_TRANSFER
    };
    struct Connection {
        Connection(int fd) : state(WS_HANDSHAKE), conn_fd(fd) {}
        bool ready() const {
            return conn_fd != -1 && state == WS_DATA_TRANSFER;
        }
        State state;
        int conn_fd;
    };

    typedef std::deque<Connection> Connections;
    typedef Connections::iterator ConnectionsIter;
    Connections connections;

    Listener& listener;

    int port;
    int server_fd;
    int random_fd;
};

}

#endif

