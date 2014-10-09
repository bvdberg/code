#ifndef SERVICE_PORT_H
#define SERVICE_PORT_H

#include <string>

namespace Luna {

class ServicePort {
public:
    ServicePort(bool creator_, const char* socketname_);
    ~ServicePort();

    int getFD() const { return fd; }

    int accept();
private:
    bool creator;
    int fd;
    std::string socketname;
};

}

#endif

