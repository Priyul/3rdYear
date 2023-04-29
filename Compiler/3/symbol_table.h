#pragma once

#include <map>
#include <string>
#include <optional>
#include <functional>
#include <stack>
#include <iostream>

#include "ast_node.h"
#include "symbol.h"

using namespace std;

class SymbolTable {
public:

    std::map<int, Symbol> table;

    void insert(int nodeId, int scopeId, const std::string &name);
    Symbol lookup(int nodeId);
    
    void initialize();

    void traverseAST(ASTNode* node, int level, std::stack<int> &scopeStack, SymbolTable &symbolTable, ASTNodeType parentType); 

    void printTable();

    bool isVariableDeclaration(ASTNode* node);

    void appendDigits(ASTNode* digitsNode, std::string& variableName);

    void analyzeScopes(ASTNode* node, int level, std::stack<int> &scopeStack, SymbolTable &symbolTable, ASTNodeType parentType);
};
