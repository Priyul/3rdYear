#include "symbol_table.h"

bool SymbolTable::isVariableDeclaration(ASTNode* node) {
    return node->type == AST_TERMINAL && (node->value == "n" || node->value == "s" || node->value == "b" || node->value == "p");
}


void SymbolTable::insert(int nodeId, int scopeId, const std::string &name) {
    Symbol symbol(nodeId, scopeId, name);
    table[nodeId] = symbol;
}

Symbol SymbolTable::lookup(int nodeId) {
    auto it = table.find(nodeId);
    if (it != table.end()) {
        // cout <<it->second.nodeId << " " << it->second.scopeId << " " << it->second.name << endl;
        return it->second;
    }

    Symbol s = Symbol(-1, -1, "");
    return s;
}


void SymbolTable::initialize() {
    table.clear();
}
void SymbolTable::appendDigits(ASTNode* digitsNode, std::string& variableName) {
    variableName += digitsNode->children[0]->value;
    if (digitsNode->children.size() == 1) {
        return;
    }
    if (!digitsNode->children.empty()) {
        appendDigits(digitsNode->children[1], variableName);
    }
}


void SymbolTable::traverseAST(ASTNode* node, int level, std::stack<int> &scopeStack, SymbolTable &symbolTable, ASTNodeType parentType) {
    if (isVariableDeclaration(node) && !node->children.empty() && node->children[0]->type == AST_DIGITS) {
        std::string variableName = node->value;

        for (const auto &child : node->children) {
            if (child->type == AST_DIGITS) {
                symbolTable.appendDigits(child, variableName);
            }
        }

        int scopeId = scopeStack.empty() ? 0 : scopeStack.top();

        bool isDuplicate = false;
        for (const auto &entry : table) {
            const Symbol &symbol = entry.second;
            if (symbol.name == variableName) {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate) {
            symbolTable.insert(node->id, scopeId, variableName);
        }


        //if (!search(node->id)) {
            // symbolTable.insert(node->id, scopeId, variableName);
        //}
        // symbolTable.insert(node->id, scopeId, variableName);
    } else if (node->type == AST_PROC) {
        int newScopeId = scopeStack.empty() ? 1 : scopeStack.top() + 1;
        scopeStack.push(newScopeId);
        symbolTable.insert(node->id, newScopeId, ""); //  fill the name later

        // Check for duplicate process names
        std::string processName = "p";
        for (const auto &child : node->children) {
            if (child->type == AST_DIGITS) {
                symbolTable.appendDigits(child, processName);
            }
        }

        bool isDuplicate = false;
        for (const auto &entry : table) {
            const Symbol &symbol = entry.second;
            if (symbol.scopeId == newScopeId && symbol.name == processName) {
                isDuplicate = true;
                break;
            }
        }

        if (isDuplicate) {
        
            symbolTable.table.erase(node->id); // Remove the inserted empty-named process
            throw std::runtime_error("Error : Duplicate process declaration for " + processName + " in the current scope!" );
        } else {
            auto symbol = symbolTable.lookup(node->id);
            symbol.name = processName;
            symbolTable.table[node->id] = symbol;
        }
    }

    for (const auto &child : node->children) {
        traverseAST(child, level + 1, scopeStack, symbolTable, node->type); // Pass the current node type as parentType
    }

    if (node->type == AST_PROC) {
        scopeStack.pop();
    }
}

void SymbolTable::printTable()  {
    cout << "Symbol Table:" << endl;
    cout << "Node ID | Scope ID | Name" << endl;
    cout << "-------------------------" << endl;
    for (const auto &entry : table) {
        const Symbol &symbol = entry.second;
        cout << symbol.nodeId << " | " << symbol.scopeId << " | " << symbol.name << endl;
    }
}

void SymbolTable::analyzeScopes(ASTNode* node, int level, std::stack<int> &scopeStack, SymbolTable &symbolTable, ASTNodeType parentType) {
    if (node->type == AST_TERMINAL && node->value == "c" && !node->children.empty() && node->children[0]->value == "p") {
        // Found a procedure call
        std::string calledProcedureName = "p";
        for (const auto &child : node->children) {
            if (child->type == AST_DIGITS) {
                symbolTable.appendDigits(child, calledProcedureName);
            }
        }

        int currentScopeId = scopeStack.empty() ? 0 : scopeStack.top();
        bool isCalledProcedureValid = false;

        // Check if the called procedure is in the current scope or in the child scope
        for (const auto &entry : table) {
            const Symbol &symbol = entry.second;
            if (symbol.name == calledProcedureName && (symbol.scopeId == currentScopeId || symbol.scopeId == currentScopeId + 1)) {
                isCalledProcedureValid = true;
                break;
            }
        }

        if (!isCalledProcedureValid) {
            std::cout << "Error: The procedure called here (" << calledProcedureName << ") has no corresponding declaration in this scope!" << std::endl;
        }
    } else if (node->type == AST_PROC) {
        int newScopeId = scopeStack.empty() ? 1 : scopeStack.top() + 1;
        scopeStack.push(newScopeId);
    }

    for (const auto &child : node->children) {
        analyzeScopes(child, level + 1, scopeStack, symbolTable, node->type); // Pass the current node type as parentType
    }

    if (node->type == AST_PROC) {
        scopeStack.pop();
    }
}


