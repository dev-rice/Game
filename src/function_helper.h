// function_helper.h
// Trevor Westphal

#ifndef FunctionHelper_h
#define FunctionHelper_h

#include <string>
#include <map>

#include "debug.h"
#include "profile.h"

class FunctionHelper;
typedef void (FunctionHelper::*function)();

class FunctionHelper {
public:
    static FunctionHelper* getInstance();
    void runFunction(std::string function_name);
private:
    static FunctionHelper* instance;
    FunctionHelper();

    void testFunction();
    void exitProgram();

    std::map<std::string, function> lookup_table;
};

#endif