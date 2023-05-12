// symbol.h
#pragma once
#include <map>
#include <string>
#include <optional>
#include <stack>
#include <iostream>

class Symbol {
public:
    Symbol(); 
    Symbol(int nodeId, int scopeId, const std::string &name);
    int nodeId;
    int scopeId;
    std::string name;
    bool isCalled;
    bool hasValue = true; // Add this line

    void setValue() { // And this method
        hasValue = true;
    }
};
