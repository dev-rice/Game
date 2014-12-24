#include "debug.h"

const char* Debug::INFO = "\033[1;36m";
const char* Debug::ERROR = "\033[1;31m";
const char* Debug::WARNING = "\033[1;33m";
const char* Debug::NORMAL = "\033[0m";

void Debug::print(const char* to_print, ...){
    // Some real hacky C shit from
    // http://stackoverflow.com/questions/5876646/how-to-overload-printf-or-cout
    va_list vl;
    va_start(vl, to_print);
    vprintf(to_print, vl);
    va_end(vl);
}
