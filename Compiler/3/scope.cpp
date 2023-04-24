#include "scope.h"

void Scope :: dfs_recursive(ASTNode* node, std::vector<Scope>& scopeStack) {
    if (node == nullptr) {
        return;
    }

    if (node->type != AST_TERMINAL) {
        // Terminal tokens generally don't need to be added to the symbol table
    } else {
        // Process non-terminal tokens
        if (node->type == AST_PROC) {
            string p = "p";
            p =  p + node->children[1]->value;
            Symbol newSymbol(p, "procedure", true, node->id);
        }
    }


}