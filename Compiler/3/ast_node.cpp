#include "ast_node.h"
#include <iostream>

using namespace std;

ASTNode::ASTNode(ASTNodeType type, const std::string& value, int id)
    : value(value), id(id) {
        // cout << "Non terminal created" << endl;
        this->type = AST_TERMINAL;
    }

ASTNode :: ASTNode(ASTNodeType type, int id) {
    this->type = type;
    this->id = id;
}

ASTNode::~ASTNode() {
    for (ASTNode* child : children) {
        delete child;
    }
}

void ASTNode::print(int level) const {
    std::string indent(level * 2, ' ');
    if (type == AST_TERMINAL) {
        std::cout << indent << "TERMINAL Node: " << value << std::endl;
    } else {
        std::cout << indent << "Node: " << type << std::endl;
    }

    for (const ASTNode* child : children) {
        child->print(level + 1);
    }
}
