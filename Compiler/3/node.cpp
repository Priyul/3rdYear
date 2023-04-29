#include "node.h"
#include "token.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

int counter = 0;



Node :: Node(NodeType type) {
    this->type = type;
    string s = "";
    this->success = false;

}

Node :: Node(NodeType type, string value) {
    this->type = type;
    this->value = value;
    //cout << "TERMINAL node created for: " << this->value << endl;
}

std::string Node :: getType(NodeType type) {
    if (type == PROGR) return "PROGR";
    if (type == PROCDEFS) return "PROCDEFS";
    if (type == PROC) return "PROC";
    if (type == DIGITS) return "DIGITS";
    if (type == D) return "D";
    if (type == MORE) return "MORE";
    if (type == ALGO) return "ALGO";
    if (type == SEQ) return "SEQ";
    if (type == INSTR) return "INSTR";
    if (type == nCALL) return "nCALL";
    if (type == ASSIGN) return "ASSIGN";
    if (type == LOOP) return "LOOP";
    if (type == BRANCH) return "BRANCH";
    if (type == nELSE) return "nELSE";
    if (type == NUMVAR) return "NUMVAR";
    if (type == nBOOLVAR) return "nBOOLVAR";
    if (type == STRINGV) return "STRINGV";
    if (type == NUMEXPR) return "NUMEXPR";
    if (type == DECNUM) return "DECNUM";
    if (type == NEG) return "NEG";
    if (type == POS) return "POS";
    if (type == INT) return "INT";
    if (type == BOOLEXPR) return "BOOLEXPR";
    if (type == LOGIC) return "LOGIC";
    if (type == CMPR) return "CMPR";
    if (type == STRI) return "STRI";
    if (type == C) return "C";
    if (type == nCOMMENT) return "nCOMMENT";
    if (type == INPUT) return "INPUT";
    if (type == OUTPUT) return "OUTPUT";
    if (type == VALUE) return "VALUE";
    if (type == TEXT) return "TEXT";
    if (type == TERMINAL) return "TERMINAL";
    return "UNKNOWN";
}

void Node :: printAST(Node* node, int level) {
    if (node == nullptr) return;

    std::string indent(level * 2, ' ');
    string type = "";

    if (node->type == PROGR) type = "PROGR";
    if (node->type == PROCDEFS) type = "PROCDEFS";
    if (node->type == PROC) type = "PROC";
    if (node->type == DIGITS) type = "DIGITS";
    if (node->type == D) type = "D";
    if (node->type == MORE) type = "MORE";
    if (node->type == ALGO) type = "ALGO";
    if (node->type == SEQ) type = "SEQ";
    if (node->type == INSTR) type = "INSTR";
    if (node->type == nCALL) type = "nCALL";
    if (node->type == ASSIGN) type = "ASSIGN";
    if (node->type == LOOP) type = "LOOP";
    if (node->type == BRANCH) type = "BRANCH";
    if (node->type == nELSE) type = "nELSE";
    if (node->type == NUMVAR) type = "NUMVAR";
    if (node->type == nBOOLVAR) type = "nBOOLVAR";
    if (node->type == STRINGV) type = "STRINGV";
    if (node->type == NUMEXPR) type = "NUMEXPR";
    if (node->type == DECNUM) type = "DECNUM";
    if (node->type == NEG) type = "NEG";
    if (node->type == POS) type = "POS";
    if (node->type == INT) type = "INT";
    if (node->type == BOOLEXPR) type = "BOOLEXPR";
    if (node->type == LOGIC) type = "LOGIC";
    if (node->type == CMPR) type = "CMPR";
    if (node->type == STRI) type = "STRI";
    if (node->type == C) type = "C";
    if (node->type == nCOMMENT) type = "nCOMMENT";
    if (node->type == INPUT) type = "INPUT";
    if (node->type == OUTPUT) type = "OUTPUT";
    if (node->type == VALUE) type = "VALUE";
    if (node->type == TEXT) type = "TEXT";
    if (node->type == TERMINAL) type = "TERMINAL";

    if (type == "TERMINAL") {
        std::cout << indent << "TERMINAL Node: " << node->value << std::endl;
    } else {
        std::cout << indent << "Node: " << type << std::endl;
    }

    for (Node *child : node->children) {
        printAST(child, level + 1);
    }
}

string Node::getType(ASTNodeType type) {
    if (type == AST_PROGR) return "PROGR";
    if (type == AST_ALGO) return "ALGO";
    if (type == AST_INSTR) return "INSTR";

    if (type == AST_COMMENT) return "COMMENT";
    if (type == AST_SEQ) return "SEQ";
    if (type == AST_PROCDEFS) return "PROCDEFS";

    if (type == AST_DECNUM) return "DECNUM";
    if (type == AST_PROC) return "PROC";
    if (type == AST_DIGITS) return "DIGITS";

    if (type == AST_TERMINAL) return "TERMINAL";
    if (type == AST_UNKNOWN) return "UNKNOWN";

}


bool Node::isPermittedNonTerminal(NodeType type) {
    // Add the permitted non-terminal node types in this list
    std::vector<NodeType> permittedNonTerminals;
    permittedNonTerminals.push_back(PROGR);
    permittedNonTerminals.push_back(ALGO);
    permittedNonTerminals.push_back(INSTR);
    permittedNonTerminals.push_back(nCOMMENT);
    permittedNonTerminals.push_back(SEQ);
    permittedNonTerminals.push_back(PROCDEFS);
    permittedNonTerminals.push_back(DECNUM);
    permittedNonTerminals.push_back(PROC);
    permittedNonTerminals.push_back(DIGITS);

    // Iterate through the permittedNonTerminals vector using a for loop
    for (size_t i = 0; i < permittedNonTerminals.size(); ++i) {
        if (permittedNonTerminals[i] == type) {
            return true;
        }
    }

    return false;
}


ASTNodeType Node::getASTNodeType(NodeType type) {
    // Implement the mapping from NodeType to ASTNodeType
    switch (type) {
        case PROGR:
            return AST_PROGR;
        case ALGO:
            return AST_ALGO;
        case INSTR:
            return AST_INSTR;
        case nCOMMENT:
            return AST_COMMENT;
        case SEQ:
            return AST_SEQ;
        case PROCDEFS:
            return AST_PROCDEFS;
        case DECNUM:
            return AST_DECNUM;
        case PROC:
            return AST_PROC;
        case DIGITS:
            return AST_DIGITS;
        case TERMINAL:
            return AST_TERMINAL;
        default:
            return AST_UNKNOWN; // Add AST_UNKNOWN to the ASTNodeType enum
    }
}

void Node::ASTtoXML(ASTNode* node, std::ostream& os, int level) {
    if (node == nullptr) return;

    std::string indent(level * 2, ' ');
    std::string tag;
    if (node->type == AST_TERMINAL) {
        tag = "TERMINAL";
    } else {
        tag = getType(node->type);
    }

    os << indent << "<Node type=\"" << tag << "\" id=\"" << node->id << "\">\n";
    os << indent << "  <Contents>" << node->value << "</Contents>\n";

    if (!node->children.empty()) {
        os << indent << "  <ChildNodes>\n";
        for (ASTNode* child : node->children) {
            os << indent << "    <ChildNodeID>" << child->id << "</ChildNodeID>\n";
        }
        os << indent << "  </ChildNodes>\n";

        // Print child nodes after closing the ChildNodes element
        for (ASTNode* child : node->children) {
            ASTtoXML(child, os, level + 1);
        }
    }

    os << indent << "</Node>\n";
}


ASTNode* Node::buildAndPrintAST(Node* parseTreeNode, int level) {
    // ASTNode* prev = nullptr;
    static int counter = 0; // Add a static counter to keep track of node IDs
    if (parseTreeNode == nullptr) return nullptr;

    ASTNode* astNode = nullptr;
    std::string astNodeType = getType(parseTreeNode->type);
    ASTNodeType astType = getASTNodeType(parseTreeNode->type);

    if (isPermittedNonTerminal(parseTreeNode->type)) {
        astNode = new ASTNode(astType, counter++);
    } else if (parseTreeNode->type == TERMINAL) {
        // cout << parseTreeNode->value << endl;
        astNode = new ASTNode(AST_TERMINAL, parseTreeNode->value, counter++);
    }

    if (astNode) {
        std::string indent(level * 2, ' ');
        if (astNode->type == AST_TERMINAL) {
            std::cout << indent << "TERMINAL Node: " << astNode->value << std::endl;
        } else {
            std::cout << indent << "Node: " << astNodeType << std::endl;
        }
    }


    for (Node* child : parseTreeNode->children) {
        ASTNode* childAstNode = buildAndPrintAST(child, level + 1);
        if (childAstNode) {
            if (astNode) {
                astNode->children.push_back(childAstNode);
            } else {
                // If the current node is not a permitted non-terminal, 
                // add its children to the parent node
                astNode = childAstNode;
            }
        }
    }

    // if (astNode) {
    //     std::string indent(level * 2, ' ');
    //     if (astNode->type == AST_TERMINAL) {
    //         std::cout << indent << "TERMINAL Node: " << astNode->value << " ID:" << astNode->id << std::endl;
    //     } else {
    //         std::cout << indent << "Node: " << astNodeType <<  " ID:" << astNode->id <<std::endl;
    //     }
    // }



    return astNode;
}

