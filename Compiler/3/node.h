#pragma once

#include "token.h"
#include "ast_node.h"
#include <vector>


enum NodeType {
    PROGR,
    PROCDEFS,
    PROC,
    DIGITS,
    D,
    MORE,
    ALGO,
    SEQ,
    INSTR,
    nCALL,
    ASSIGN,
    LOOP,
    BRANCH,
    nELSE,
    NUMVAR,
    nBOOLVAR,
    STRINGV,
    NUMEXPR,
    DECNUM,
    NEG,
    POS,
    INT,
    BOOLEXPR,
    LOGIC,
    CMPR,
    STRI,
    C,
    nCOMMENT,
    INPUT,
    OUTPUT,
    VALUE,
    TEXT,
    TERMINAL
};

class Node {
public:
    NodeType type;
    std::string value;
    std::vector<Node*> children;

    Node(NodeType type);
    Node(NodeType type, string value);
    virtual ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }

    void printAST(Node* node, int level = 0);
    std::string getType(NodeType type);
    

    ASTNode* buildAndPrintAST(Node* parseTreeNode, int level = 0);
    bool isPermittedNonTerminal(NodeType type);
    ASTNodeType getASTNodeType(NodeType type);
    string getType(ASTNodeType type);

    void ASTtoXML(ASTNode* node, std::ostream& os, int level = 0);

    bool success;

};


