// semantic_analyzer.cpp
#include "semantic_analyzer.h"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer(ASTNode* ast) {
    this->ast = ast;
}

void SemanticAnalyzer::analyze(ASTNode* ast) {
    check_rule1(ast);
    // ... And so on for each rule
}

void SemanticAnalyzer::check_rule1(ASTNode* ast) {
    // Traverse the AST and check for violations of rule 1
    // If a violation is found, add an error message to the errors vector
    // If a variable is assigned a value, flip its hasValue flag in the symbol table
}

// ... And so on for each rule

void SemanticAnalyzer::reportErrors() {
    if (errors.empty()) {
        std::cout << "No errors found.\n";
    } else {
        std::cout << "Errors found:\n";
        for (const auto& error : errors) {
            std::cout << "- " << error << "\n";
        }
    }
}
