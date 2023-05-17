#include "scope_analyzer.h"

ScopeAnalyzer::ScopeAnalyzer(ASTNode* root) : root(root), current_scope_id(0) {}

void ScopeAnalyzer::analyze() {
    traverse(root, 0);
}

SymbolTable ScopeAnalyzer::getSymbolTable() {
    return symbol_table;
}

void ScopeAnalyzer::traverse(ASTNode* node, int current_scope) {
    if (node->type == AST_PROC) {
        current_scope = ++current_scope_id;
    }

    symbol_table.insert(node->id, current_scope, node->value);

    for (ASTNode* child : node->children) {
        traverse(child, current_scope);
    }
}
