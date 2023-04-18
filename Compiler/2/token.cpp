#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include "token.h"

Token :: Token(TokenType type, char value) {
        this->type = type;
        this->value = value;
        //cout << "Token: " << value << "\tType: " << type << endl;
    };

std::string Token :: toString(TokenType type) {
    if (type == COMMA) {
        return "Comma";
    } else if (type == PROCESS) {
        return "Process";
    } else if (type == LEFT_BRACE) {
        return "Left Brace";
    } else if (type == RIGHT_BRACE) {
        return "Right Brace";
    } else if (type == DIGIT) {
        return "Digit";
    } else if (type == SEMICOLON) {
        return "Semicolon";
    } else if (type == HALT) {
        return "Halt";
    } else if (type == CALL) {
        return "Call";
    } else if (type == ASSIGN_OP) {
        return "Assign";
    } else if (type == WHILE) {
        return "While";
    } else if (type == LEFT_PAREN) {
        return "Left Parenthesis";
    } else if (type == RIGHT_PAREN) {
        return "Right Parenthesis";
    } else if (type == IF) {
        return "If";
    } else if (type == THEN) {
        return "Then";
    } else if (type == ELSE) {
        return "Else";
    } else if (type == NUMERIC_VAR) {
        return "Numeric Variable";
    } else if (type == BOOLVAR) {
        return "Boolean Variable";
    } else if (type == STRINGVAR) {
        return "String Variable";
    } else if (type == ADD_OP) {
        return "Addition Operator";
    } else if (type == MULTI_OP) {
        return "Multiplication Operator";
    } else if (type == DIV_OP) {
        return "Division Operator";
    } else if (type == DECIMAL) {
        return "Decimal";
    } else if (type == MINUS) {
        return "Minus";
    } else if (type == DOT) {
        return "Dot";
    } else if (type == INTEGER) {
        return "Integer";
    } else if (type == TRUE) {
        return "True";
    } else if (type == FALSE) {
        return "False";
    } else if (type == AND) {
        return "And";
    } else if (type == OR) {
        return "Or";
    } else if (type == NOT) {
        return "Not";
    } else if (type == EQUAL) {
        return "Equal";
    } else if (type == NOT_EQUAL) {
        return "Not Equal";
    } else if (type == LESS_THAN) {
        return "Less Than";
    } else if (type == LESS_THAN_EQUAL) {
        return "Less Than or Equal";
    } else if (type == GREATER_THAN) {
        return "Greater Than";
    } else if (type == GREATER_THAN_EQUAL) {
        return "Greater Than or Equal";
    } else if (type == STRING) {
        return "String";
    } else if (type == COMMENT) {
        return "Comment";
    } else if (type == INPUT_G) {
        return "Input";
    } else if (type == OUTPUT_O) {
        return "Output";
    } else if (type == LETTER) {
        return "Letter";
    } else if (type == RESPONSE) {
        return "Response";
    } else if (type == COMMENT_END) {
        return "Comment End";
    } else {
        return "Unknown";
    }
}
    
std::string Token :: getType() {
    return toString(type);
}

std::string Token :: getValue() {
    std::string myString(1, this->value);
    return myString;
}
