#pragma once

#include "ast_node.h"
#include "symbol_table.h"

class ScopeAnalyzer {
public:
    ScopeAnalyzer(ASTNode* root);

    void analyze();
    SymbolTable getSymbolTable();

private:
    void traverse(ASTNode* node, int current_scope);

    ASTNode* root;
    SymbolTable symbol_table;
    int current_scope_id;
};
