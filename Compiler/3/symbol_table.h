#pragma once

#include <unordered_map>
#include "ast_node.h"

struct Symbol {
    int id;
    int scope_id;
    ASTNodeType type;
    std::string name;
};

class SymbolTable {
public:
    void insert(int node_id, int scope_id, ASTNodeType type, const std::string &name);
    Symbol* find(int node_id);

private:
    std::unordered_map<int, Symbol> table;
};
