#ifndef ASPECTPARSER_H
#define ASPECTPARSER_H

#include <string>
#include <list>
#include <stack>
#include <sstream>

class AspectParserException : public std::exception {
public:
    AspectParserException(const std::string& msg_) : msg(msg_) {}
    virtual ~AspectParserException() throw() {}
    virtual const char* what() const throw() { return msg.c_str(); }
private:
    std::string msg;
};


typedef std::list<std::string> Aspects;
typedef Aspects::const_iterator AspectsIter;


class AspectParser {
public:
    AspectParser(const char* filename, const Aspects& aspects_);
protected:
    void error(const std::string& msg);
    bool updateState();
    void readLine(char* input, std::stringstream& output);

    typedef std::stack<std::string> Stack;
    Stack aspectStack;

    const Aspects& aspects;

    unsigned int line;
    bool copyInput;
};

#endif

