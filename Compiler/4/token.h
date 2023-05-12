#pragma once

#include <string>
#include <iostream>
#include <cctype>
#include <vector>


using namespace std;

enum TokenType {
        COMMA, // ,
    PROCESS, //pDIGITS
        LEFT_BRACE, // {
        RIGHT_BRACE, // }
    DIGIT, // 0-9
        SEMICOLON, // ;
        HALT, //h
        CALL, //c
        ASSIGN_OP, // :=
        WHILE, //w
        LEFT_PAREN, // (
        RIGHT_PAREN, // )
        IF, //i 
        THEN, //t
        ELSE, //e

        NUMERIC_VAR, //n
        BOOLVAR, //b
        STRINGVAR, //s
        ADD_OP, //a
        MULTI_OP, //m
        DIV_OP, //d
        DECIMAL, // 0.
        MINUS, // -
        DOT, // .
    INTEGER, // 1MORE|2MORE|3MORE|4MORE|5MORE|6MORE|7MORE|8MORE|9MORE (MORE = DIGITS = D MORE = 0|1|2|3|4|5|6|7|8|9)
        TRUE, //T
        FALSE, //F
        AND, //^
        OR, //v
        NOT, //!
        EQUAL, //E
    NOT_EQUAL, //!E
        LESS_THAN, // <
    LESS_THAN_EQUAL, //<E
        GREATER_THAN, //>
    GREATER_THAN_EQUAL, //>E
    STRING, //" QUOTE "
    COMMENT, // * COMMENT *
        INPUT_G, //g
        OUTPUT_O, //o
        RESPONSE, //r
        LETTER, // a-z/A-Z/_
        COMMENT_END


};

class Token {
public:
    TokenType type;
    char value;
    Token(TokenType type, char value);
    std::string toString(TokenType type);
    std::string getType();
    std::string getValue();
};