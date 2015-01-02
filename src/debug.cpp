#include "debug.h"

const char* Debug::INFO = "\033[1;36m";
const char* Debug::ERROR = "\033[1;31m";
const char* Debug::WARNING = "\033[1;33m";
const char* Debug::NORMAL = "\033[0m";

bool Debug::is_on;

void Debug::info(const char* input, ...){
    if (!Debug::is_on){
        return;
    }

    char buffer[256];
    sprintf(buffer, "%s[info]%s %s", Debug::INFO, Debug::NORMAL, input);
    input = buffer;

    va_list argument_list;
    va_start(argument_list, input);
    vprintf(input, argument_list);
    va_end(argument_list);
}

void Debug::error(const char* input, ...){
    if (!Debug::is_on){
        return;
    }

    char buffer[256];
    sprintf(buffer, "%s[error]%s %s", Debug::ERROR, Debug::NORMAL, input);
    input = buffer;

    va_list argument_list;
    va_start(argument_list, input);
    vprintf(input, argument_list);
    va_end(argument_list);
}

void Debug::warning(const char* input, ...){
    if (!Debug::is_on){
        return;
    }

    char buffer[256];
    sprintf(buffer, "%s[warning]%s %s", Debug::WARNING, Debug::NORMAL, input);
    input = buffer;

    va_list argument_list;
    va_start(argument_list, input);
    vprintf(input, argument_list);
    va_end(argument_list);
}

void Debug::print(const char* to_print, ...){
    // Some real hacky C shit from
    // http://stackoverflow.com/questions/5876646/how-to-overload-printf-or-cout
    va_list argument_list;
    va_start(argument_list, to_print);
    vprintf(to_print, argument_list);
    va_end(argument_list);
}
