#include "symbol_table.h"

void SymbolTable::insert(int node_id, int scope_id, ASTNodeType type, const std::string &name) {
    Symbol symbol = {node_id, scope_id, type, name};
    table[node_id] = symbol;
}

Symbol* SymbolTable::find(int node_id) {
    auto it = table.find(node_id);
    if (it != table.end()) {
        return &(it->second);
    }
    return nullptr;
}
