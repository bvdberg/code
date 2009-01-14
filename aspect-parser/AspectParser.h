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


class AspectParser {
public:
    AspectParser();
    void addAspect(const std::string& aspect);
    void parse(const char* filename);
protected:
    void error(const std::string& msg);
    bool updateState();
    void readLine(char* input, std::stringstream& output);

    typedef std::stack<std::string> Stack;
    Stack aspectStack;

    typedef std::list<std::string> Aspects;
    typedef Aspects::const_iterator AspectsIter;
    Aspects aspects;

    unsigned int line;
    bool copyInput;
};

#endif

