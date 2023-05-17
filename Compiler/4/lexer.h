#pragma once

#include <string>
#include <vector>
#include "token.h"

class Lexer {
public:
    Lexer(std::string input1);
    std::vector<Token> tokenize(string i);

    std::string getInput();
    int getPosition();
    
    void advance();
    void unadvance();
    bool isWhiteSpace(char s);
    bool isdigit(char s);
    bool isascii(char s);
    void report(string why);

    bool isasciistr(char s);
    bool isasciicom(char s);

    std::vector<Token> getTokens();
    bool isLexError = false;
private:
    std::string input1;
    int position;
    std::vector<Token> tokens;

};
