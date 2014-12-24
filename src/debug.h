#ifndef Debug_h
#define Debug_h

#include <cstdarg>
#include <stdarg.h>
#include <stdio.h>

class Debug{
public:
    static void info(const char*, ...);
    static void error(const char*, ...);
    static void print(const char*, ...);

private:
    static const char* INFO;
    static const char* ERROR;
    static const char* WARNING;
    static const char* NORMAL;

};


#endif
