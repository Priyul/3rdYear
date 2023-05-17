// semantic_analyzer.h
#pragma once
#include "ast_node.h"
#include "symbol.h"
#include <map>
#include <string>
#include <vector>

class SemanticAnalyzer {
public:
    SemanticAnalyzer(ASTNode* ast);
    void analyze(ASTNode* ast);
    void reportErrors();

private:
    std::map<std::string, Symbol> symbolTable;
    std::vector<std::string> errors;

    void check_rule1(ASTNode* ast);

    ASTNode* ast;
    // ... And so on for each rule
};
