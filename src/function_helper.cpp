// function_helper.cpp
// Trevor Westphal

#include "function_helper.h"

FunctionHelper* FunctionHelper::instance;

FunctionHelper* FunctionHelper::getInstance(){
    if(instance){
        return instance;
    } else {
        instance = new FunctionHelper();
        return instance;
    }
}

FunctionHelper::FunctionHelper(){
    lookup_table["testFunction"] = &FunctionHelper::testFunction;
    lookup_table["exitProgram"] = &FunctionHelper::exitProgram;
    lookup_table["toggleShadows"] = &FunctionHelper::toggleShadows;
}

void FunctionHelper::runFunction(std::string function_name){
    std::map<std::string, function>::iterator result = lookup_table.find(function_name);

    if(result != lookup_table.end()){
        auto mem = result->second;  //C++11 only
        (this->*mem)();
    } else {
        Debug::error("Could not run requested function: %s\n", function_name.c_str());
    }
}

void FunctionHelper::testFunction(){
    Profile::getInstance()->toggleVsync();
}

void FunctionHelper::toggleShadows(){
    Profile::getInstance()->toggleShadows();
}

void FunctionHelper::exitProgram(){
    Window::getInstance()->requestClose();
}
