#ifndef TCP_SERVER_H
#define TCP_SERVER_H

class TcpServer {
public:
    TcpServer(int port_);
    ~TcpServer ();

    int getFD() const { return fd; }

    int accept(char** src_ipnr);
private:
    int port;
    int fd;
};

#endif

