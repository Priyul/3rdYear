#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>   
#include <map>
#include <unordered_map>
#include <cstdlib> 

#include "node.h"
#include "ast_node.h"

using namespace std;

class Symbol {
public:
    string name;
    std::string type;
    bool isProcedure; // True for procedure symbols, false for variable symbols
    int scopeId;


    Symbol(std::string name, std::string type, bool isProcedure)
        : name(name), type(type), isProcedure(isProcedure), scopeId(scopeId) {}
};

class Scope {
public:
    std::string name;
    std::unordered_map<std::string, Symbol> symbols;

    Scope(std::string name) : name(name) {}

    void dfs_recursive(ASTNode* node, std::vector<Scope>& scopeStack);
};
