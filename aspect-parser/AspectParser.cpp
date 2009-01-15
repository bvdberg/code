#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

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

AspectParser::AspectParser()
    : line(1), copyInput(true)
{}


void AspectParser::addAspect(const std::string& aspect) {
    // TODO filter duplicate entries
    aspects.push_back(aspect);
}


void AspectParser::parseFile(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd== -1) throw AspectParserException("error opening file");

    struct stat buf;
    int result = fstat(fd, &buf);
    if (result == -1) throw AspectParserException("error statting file");
    int size =  buf.st_size;
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE;
    void* map = mmap(NULL, size, prot, flags, fd, 0);
    if (map == (void*)-1) {
        throw AspectParserException("error creating map");
    }
    close(fd);

    parse((char*)map, size); 
    munmap(map, size);
}


void AspectParser::parse(char* start, unsigned int size) {
    copyInput = true;
    line = 1;

    std::stringstream outputBuffer;

    char* end = start + size;
    char* input = start;
    char* lineStart = input;
    while (input != end) {
        if (*input == LF) {
            *input = 0;

            readLine(lineStart, outputBuffer);
            line++;
            lineStart = input+1;
        }
        input++;
    }

    if (!aspectStack.empty()) {
        string name = aspectStack.front();
        error("missing end of aspect: '" + name + "' at end of file");
    }

    cout << outputBuffer.str();
}


void AspectParser::error(const string& msg) {
    throw AspectParserException(msg + " (line: " + itoa(line) + ")");
}


bool AspectParser::updateState() {
    if (aspectStack.empty()) return true;

    string currentAspect = aspectStack.front();
    
    AspectsIter iter = aspects.begin();
    while (iter != aspects.end()) {
        if (*iter == currentAspect) return true;
        ++iter;   
    }
    return false;
}


void AspectParser::beginAspect(const std::string& name) {
    printf("DEBUG: %s(%s)\n", __func__, name.c_str());
    for (StackConstIter iter = aspectStack.begin(); iter != aspectStack.end(); iter++) {
        if (*iter == name) {
            error("aspect '" + name + "' already started");
            break;
        }
    }
    aspectStack.push_front(string(name));
    copyInput = updateState();
}


void AspectParser::endAspect(const std::string& name) {
    printf("DEBUG: %s(%s)\n", __func__, name.c_str());
    if (aspectStack.empty()) error("aspect '" + name + "' has missing begin");
    string top = aspectStack.front();
    if (top != name) error("aspect '" + name + "' has missing begin");
        
    aspectStack.pop_front();
    copyInput = updateState();
}


void AspectParser::readLine(char* input, stringstream& output) {
    if (strncmp("%%begin ", input, 8) == 0) {
        string name(input + 8);
        beginAspect(name);
    } else if (strncmp("%%end ", input, 6) == 0) {
        string name(input + 6);
        endAspect(name);
    } else if (copyInput) {
        output << input << '\n';
    }
}

