#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdarg.h>

#include "AspectParser.h"

using namespace std;

static inline char* itoa(unsigned int value) {
    static char str[11];
    str[10] = '\0';
    int  pos = 9;
    do {
        str[pos--] = (char) ((value % 10) + 0x30);
        value /= 10;
    } while (value != 0);
    return str + pos + 1;
}

#define LF 10
#define CR 13

AspectParser::AspectParser(const char* filename, const Aspects& aspects_)
        : aspects(aspects_), line(1), copyInput(true)
{
    FILE* file = fopen(filename, "r");
    if (file == 0) throw AspectParserException("error opening file");

    char* buffer = 0;
    size_t bufsize = 0;

    std::stringstream outputBuffer;

    while (true) {
        ssize_t size = getline(&buffer, &bufsize, file);
        if (size == -1) break;
        if (buffer[size-2] == CR) buffer[size-2] = 0;
        if (buffer[size-1] == LF) buffer[size-1] = 0;
        readLine(&buffer[0], outputBuffer);
        ++line;
    }
    if (!aspectStack.empty()) {
        string name = aspectStack.top();
        error("missing end of aspect: '" + name + "' at end of file");
    }

    if (buffer) free(buffer);
    fclose(file);

    cout << outputBuffer.str();
}


void AspectParser::error(const string& msg) {
    throw AspectParserException(msg + " (line: " + itoa(line) + ")");
}


bool AspectParser::updateState() {
    if (aspectStack.empty()) return true;

    string currentAspect = aspectStack.top();
    
    AspectsIter iter = aspects.begin();
    while (iter != aspects.end()) {
        if (*iter == currentAspect) return true;
        ++iter;   
    }
    return false;
}


void AspectParser::readLine(char* input, stringstream& output) {
    if (copyInput && strncmp("%%begin ", input, 8) == 0) {
        string name(input + 8);
        if (!aspectStack.empty()) {
            string top = aspectStack.top();
            if (top == name) error("aspect '" + name + "' already started");
        }
        aspectStack.push(string(name));
        copyInput = updateState();
    } else if (strncmp("%%end ", input, 6) == 0) {
        string name(input + 6);
        if (aspectStack.empty()) error("aspect '" + name + "' has missing begin");
        string top = aspectStack.top();
        if (!copyInput && name != top) return;
        if (top != name) error("aspect '" + name + "' has missing begin");
        aspectStack.pop();
        copyInput = updateState();
    } else if (copyInput) {
        output << input << '\n';
    }
}

