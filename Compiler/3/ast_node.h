#pragma once

#include <string>
#include <vector>

enum ASTNodeType {
    AST_PROGR,
    AST_ALGO,
    AST_INSTR,
    AST_COMMENT,
    AST_SEQ,
    AST_PROCDEFS,
    AST_DECNUM,
    AST_PROC,
    AST_DIGITS,
    AST_TERMINAL,
    AST_UNKNOWN
};

class ASTNode {
public:
    ASTNodeType type;
    std::string value;
    std::vector<ASTNode*> children;
    int id;

    ASTNode(ASTNodeType type, int id);
    ASTNode(ASTNodeType type, const std::string &value, int id);
    ~ASTNode();

    void print(int level = 0) const;
};
