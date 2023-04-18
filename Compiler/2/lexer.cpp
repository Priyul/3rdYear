#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <cstdlib>

#include <regex>
#include <iterator>
#include "token.h"
#include "lexer.h"

using namespace std;

Lexer::Lexer(std::string i1) {
    this->input1 = i1;
    this->position = 0;
    if (isWhiteSpace(this->input1[position])) { //skip white spaces if they are at the start of a program.
        while (isWhiteSpace(this->input1[position])) {
            this->position++;
        }
    }
}

void Lexer :: report(string why) {
    throw std::runtime_error("Lexical error: " + why);
}

void Lexer :: advance() {
    // cout << position << endl;
    position++;
    while (isWhiteSpace(input1[position])) {
        position++;
    }
}

void Lexer :: unadvance() {
    position--;
    while (isWhiteSpace(input1[position])) {
        position--;
    }
}

bool Lexer :: isWhiteSpace(char s) {
    if (s == ' ' || s == '\t' || s == '\n') {
        return true;
    } else {
        return false;
    }
}

bool Lexer :: isdigit(char  s) {
    if (s == '0' || s == '1' || s == '2' || s == '3' || s == '4' || s == '5' || s == '6' || s == '7' || s == '8' || s == '9') {
        return true;
    } else {
        return false;
    }
}

bool Lexer :: isasciicom(char s) {
    // if (s == 'a' || s == 'b' || s == 'c' || s == 'd' || s == 'e' || s == 'f' 
    // || s == 'g' || s == 'h' || s == 'i' || s == 'j' || s == 'k' || s == 'l' 
    // || s == 'm' || s == 'n' || s == 'o' || s == 'p' || s == 'q' || s == 'r' 
    // || s == 's' || s == 't' || s == 'u' || s == 'v' || s == 'w' || s == 'x' 
    // || s == 'y' || s == 'z' || s == 'A' || s == 'B' || s == 'C' || s == 'D' 
    // || s == 'E' || s == 'F' || s == 'G' || s == 'H' || s == 'I' || s == 'J' 
    // || s == 'K' || s == 'L' || s == 'M' || s == 'N' || s == 'O' || s == 'P' 
    // || s == 'Q' || s == 'R' || s == 'S' || s == 'T' || s == 'U' || s == 'V' 
    // || s == 'W' || s == 'X' || s == 'Y' || s == 'Z' || s == '0' || s == '1' 
    // || s == '2' || s == '3' || s == '4' || s == '5' || s == '6' || s == '7' 
    // || s == '8' || s == '9' || s == '_' || s == '!' || s == '@' || s == '#'
    // || s == '!' || s == '\"' || s == '$' || s == '%' || s == '&' || s == '(' 
    // || s == ')' || s == '+' || s == ',' || s == '-' || s == '.' ||
    // s == '/' || s == ':' || s == ';' || s == '<' || s == '=' || s == '>' || s == '?'
    // || s == '[' || s == ']' || s == '^' || s == '`' || s == '{' || s == '|' || s == '}'
    //  || s == '~' || s == 32 || s == 39 || s == 134) {
    //     return true;
    // } else {
    //     return false;
    // }

    if (s >= 32 && s <= 127 && s != '*'){
        return true;
    } else {
        return false;
    }
}

bool Lexer :: isasciistr(char s) {
    // if (s == 'a' || s == 'b' || s == 'c' || s == 'd' || s == 'e' || s == 'f' 
    // || s == 'g' || s == 'h' || s == 'i' || s == 'j' || s == 'k' || s == 'l' 
    // || s == 'm' || s == 'n' || s == 'o' || s == 'p' || s == 'q' || s == 'r' 
    // || s == 's' || s == 't' || s == 'u' || s == 'v' || s == 'w' || s == 'x' 
    // || s == 'y' || s == 'z' || s == 'A' || s == 'B' || s == 'C' || s == 'D' 
    // || s == 'E' || s == 'F' || s == 'G' || s == 'H' || s == 'I' || s == 'J' 
    // || s == 'K' || s == 'L' || s == 'M' || s == 'N' || s == 'O' || s == 'P' 
    // || s == 'Q' || s == 'R' || s == 'S' || s == 'T' || s == 'U' || s == 'V' 
    // || s == 'W' || s == 'X' || s == 'Y' || s == 'Z' || s == '0' || s == '1' 
    // || s == '2' || s == '3' || s == '4' || s == '5' || s == '6' || s == '7' 
    // || s == '8' || s == '9' || s == '_' || s == '!' || s == '@' || s == '#'
    // || s == '!' || s == '$' || s == '%' || s == '&' || s == '(' 
    // || s == ')' || s == '*' || s == '+' || s == ',' || s == '-' || s == '.' ||
    // s == '/' || s == ':' || s == ';' || s == '<' || s == '=' || s == '>' || s == '?'
    // || s == '[' || s == ']' || s == '^' || s == '`' || s == '{' || s == '|' || s == '}'
    //  || s == '~' || s == 39 || s == 32 || s == 92) {
    //     return true;
    // } else {
    //     return false;
    // }

    if (s >= 32 && s <= 127 && s != '/"'){
        return true;
    } else {
        return false;
    }
}

bool Lexer :: isascii(char s) {
    if (s == 'a' || s == 'b' || s == 'c' || s == 'd' || s == 'e' || s == 'f' 
    || s == 'g' || s == 'h' || s == 'i' || s == 'j' || s == 'k' || s == 'l' 
    || s == 'm' || s == 'n' || s == 'o' || s == 'p' || s == 'q' || s == 'r' 
    || s == 's' || s == 't' || s == 'u' || s == 'v' || s == 'w' || s == 'x' 
    || s == 'y' || s == 'z' || s == 'A' || s == 'B' || s == 'C' || s == 'D' 
    || s == 'E' || s == 'F' || s == 'G' || s == 'H' || s == 'I' || s == 'J' 
    || s == 'K' || s == 'L' || s == 'M' || s == 'N' || s == 'O' || s == 'P' 
    || s == 'Q' || s == 'R' || s == 'S' || s == 'T' || s == 'U' || s == 'V' 
    || s == 'W' || s == 'X' || s == 'Y' || s == 'Z' || s == '0' || s == '1' 
    || s == '2' || s == '3' || s == '4' || s == '5' || s == '6' || s == '7' 
    || s == '8' || s == '9' || s == '_' || s == '!' || s == '@' || s == '#'
    || s == '!' || s == '\"' || s == '$' || s == '%' || s == '&' || s == '(' 
    || s == ')' || s == '*' || s == '+' || s == ',' || s == '-' || s == '.' ||
    s == '/' || s == ':' || s == ';' || s == '<' || s == '=' || s == '>' || s == '?'
    || s == '[' || s == ']' || s == '^' || s == '`' || s == '{' || s == '|' || s == '}'\
     || s == '~' || s == 32 || s == 92) {
        return true;
    } else {
        return false;
    }

}

std::vector<Token> Lexer :: getTokens() {
    return this->tokens;
}

std::string Lexer :: getInput() {
    return input1;
}

int Lexer :: getPosition() {
    return position;
}

std::vector<Token> Lexer::tokenize(string i) {
    cout << "Tokenizing input..." << endl;

    while (position < i.size()) {
        if (i[position] == ',') {
            Token t(COMMA, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == 'p') {
            advance();
            if (isdigit(i[position])) {
                Token t(PROCESS, i[position-1]);
                tokens.push_back(t);
                continue;
            } else {
                unadvance();
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
                continue;
            } 
        } else if (i[position] == '0') {
            if (isdigit(i[position-1])) {
                //eg) 10.xx
                Token t(DIGIT, i[position]);
                tokens.push_back(t);
                advance();
            } else if (isdigit(i[position+1]) && !isdigit(i[position-1]) && i[position-1] != '.') {
                //eg) 01.21
                report("Invalid INT, cannot start with a 0");
            } else if (i[position+1] == '.') {
                //eg) 0.x
                if (i[position+2] == '0') {
                    if (i[position+3] == '0') {
                        //eg) 0.00
                        Token t(DECIMAL, '0');
                        tokens.push_back(t);
                        advance();
                        advance();
                        advance();
                        advance();
                    } else {
                        report("Invalid DECIMAL, must have 2 digits after the decimal point");
                    }
                } else if (isdigit(i[position+2]) && i[position+1] == '.') {
                    //eg) 0.21
                    report("Invalid INT, cannot start with a 0");

                } else {
                    report("Invalid DECIMAL, must have 2 digits after the decimal point");
                }
            } else {
                //eg) 102.89
                Token t(DIGIT, i[position]);
                tokens.push_back(t);
                advance();
                continue;
            }
        } else if (i[position] == '0' || i[position] == '1' || i[position] == '2' || i[position] == '3' || i[position] == '4' || i[position] == '5' || i[position] == '6' || i[position] == '7' || i[position] == '8' || i[position] == '9') {
            Token t(DIGIT, i[position]);
            tokens.push_back(t);
            advance();
            continue;
        } else if (i[position] == '.') {
            Token t(DOT, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == ';') {
            Token t(SEMICOLON, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == 'h') {
            Token t(HALT, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == 'c') {
            Token t(CALL, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == '{') {
            Token t(LEFT_BRACE, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == '}') {
            Token t(RIGHT_BRACE, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == '(') {
            Token t(LEFT_PAREN, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == ')') {
            Token t(RIGHT_PAREN, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == ':') {
            advance();
            if (i[position] == '=') {
                // Token t1(ASSIGN_OP, ':');
                Token t2(ASSIGN_OP, '=');
                // tokens.push_back(t1);
                tokens.push_back(t2);
                advance();
            } else {
                report("Expected '=' after ':'");
            }
        } else if (i[position] == 'w') {
            bool isCurlyBrace = false;
            // cout << "here" << endl;
            if (position >= 0) {
                advance();
                char prevValue = i[position];
                // cout << "Hi" << endl;
                isCurlyBrace = (prevValue == '(');
                unadvance();
            }
  
            if (isCurlyBrace) {
                Token t(WHILE, 'w');
                tokens.push_back(t);
                advance();
            } else if (!isCurlyBrace) {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
            } else {
                advance();
            }
        } else if (i[position] == '(') {
            Token t(LEFT_PAREN, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == ')') {
            Token t(RIGHT_PAREN, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == 'i') {
            bool isCurlyBrace = false;
            if (position > 0) {
                advance();
                char prevValue = i[position];
                isCurlyBrace = (prevValue == '(');
                unadvance();
            }
  
            if (isCurlyBrace) {
                Token t(IF, 'i');
                tokens.push_back(t);
                advance();
            } else if (!isCurlyBrace) {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
            } else {
                advance();
            }
        } else if (i[position] == 't') {
            bool isCurlyBrace = false;
            if (position > 0) {
                advance();
                char prevValue = i[position];
                isCurlyBrace = (prevValue == '{');
                unadvance();
            }
  
            if (isCurlyBrace) {
                Token t(THEN, 't');
                tokens.push_back(t);
                advance();
            } else if (!isCurlyBrace) {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
            } else {
                advance();
            }
        } else if (i[position] == 'e') {
            bool isCurlyBrace = false;
            if (position > 0) {
                advance();
                char prevValue = i[position];
                isCurlyBrace = (prevValue == '{');
                unadvance();
            }
  
            if (isCurlyBrace) {
                Token t(ELSE, 'e');
                tokens.push_back(t);
                advance();
            } else if (!isCurlyBrace) {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
            } else {
                advance();
            }
        } else if (i[position] == 'n') {
            if (isdigit(i[position+1])) {
                Token t(NUMERIC_VAR, 'n');
                tokens.push_back(t);
                advance();
                continue;
            } else {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
                continue;
            }
        } else if (i[position] == 'b') {
            if (isdigit(i[position+1])) {
                Token t(BOOLVAR, 'b');
                tokens.push_back(t);
                advance();
                continue;
            } else {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
                continue;
            }
        } else if (i[position] == 's') {
            if (isdigit(i[position+1])) {
                Token t(STRINGVAR, 's');
                tokens.push_back(t);
                advance();
                continue;
            } else {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
                continue;
            }
        } else if (i[position] == 'a') {
            bool isPartOfWord = true;
            if (position > 0) {
                string val1 = i.substr(position+1, 1);
                // cout << position << " and val1: " << val1;
                string val2 = i.substr(position+2, 1);
                // cout << " and val2: " << val2 << endl;
                if (val1 == "(" && (val2 == "n" || val2 == "0" || val2 == "-" || val2 == "1" || val2 == "2" || 
                val2 == "3" || val2 == "4" || val2 == "5" || val2 == "6" || val2 == "7" || val2 == "8" || 
                val2 == "9" ||  val2 == "a" || val2 == "m" || val2 == "d")) {
                    isPartOfWord = false;
                }
            }
  
            if (!isPartOfWord) {
                Token t(ADD_OP, 'a');
                tokens.push_back(t);
                advance();
            } else if (isPartOfWord) {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
            } else {
                advance();
            }
        } else if (i[position] == 'm') {
            bool isPartOfWord = true;
            if (position > 0) {
                string val1 = i.substr(position+1, 1);
                string val2 = i.substr(position+2, 1);
                if (val1 == "(" && (val2 == "n" || val2 == "0" || val2 == "-" || val2 == "1" || val2 == "2" || 
                val2 == "3" || val2 == "4" || val2 == "5" || val2 == "6" || val2 == "7" || val2 == "8" || 
                val2 == "9" || val2 == "a" || val2 == "m" || val2 == "d")) {
                    isPartOfWord = false;
                }
            }
  
            if (!isPartOfWord) {
                Token t(MULTI_OP, 'm');
                tokens.push_back(t);
                advance();
            } else if (isPartOfWord) {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
            } else {
                advance();
            }
        } else if (i[position] == 'd') {
            bool isPartOfWord = true;
            if (position > 0) {
                string val1 = i.substr(position+1, 1);
                string val2 = i.substr(position+2, 1);
                if (val1 == "(" && (val2 == "n" || val2 == "0" || val2 == "-" || val2 == "1" || val2 == "2" || 
                val2 == "3" || val2 == "4" || val2 == "5" || val2 == "6" || val2 == "7" || val2 == "8" || 
                val2 == "9" || val2 == "a" || val2 == "m" || val2 == "d")) {
                    isPartOfWord = false;
                }
            }
  
            if (!isPartOfWord) {
                Token t(DIV_OP, 'd');
                tokens.push_back(t);
                advance();
            } else if (isPartOfWord) {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
            } else {
                advance();
            }
        } else if (i[position] == '-') {
            Token t(MINUS, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == 'T') {
            Token t(TRUE, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == 'F') {
            Token t(FALSE, i[position]);
            tokens.push_back(t);
            advance();
        } else if (i[position] == '^') {
            int count = 0;
            advance();
            count++;
            if (i.substr(position, 1) == "(") {
                Token t(AND, '^');
                tokens.push_back(t);
                for (int i = 0; i < count; i++) {
                    unadvance();
                }
                advance();
            } else {
                report("Expected ^(, recieved ^" + i.substr(position, 1));
            }
        } else if (i[position] == 'v') {
            int count = 0;
            advance();
            count++;
            if (i.substr(position, 1) == "(") {
                Token t(OR, 'v');
                tokens.push_back(t);
                for (int i = 0; i < count; i++) {
                    unadvance();
                }
                advance();
            } else if (isascii(i[position])) {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                for (int i = 0; i < count; i++) {
                    unadvance();
                }
                advance();
            } else {
                report("Expected v(, recieved v" + i.substr(position, 1));
            }
        } else if (i[position] == '!') {
            int count = 0;
            advance();
            count++;
            if (i.substr(position, 1) == "(") {
                Token t(NOT, '!');
                tokens.push_back(t);
                for (int i = 0; i < count; i++) {
                    unadvance();
                }
                advance();
            } else {
                report("Expected !(, recieved !" + i.substr(position, 1));
            }
        } else if (i[position] == '<') {
            int count = 0;
            advance();
            count++;
            if (i.substr(position, 1) == "(") {
                Token t(LESS_THAN, '<');
                tokens.push_back(t);
                for (int i = 0; i < count; i++) {
                    unadvance();
                }
                advance();
            } else {
                report("Expected <(, recieved <" + i.substr(position, 1));
            }
        } else if (i[position] == '>') {
            int count = 0;
            advance();
            count++;
            if (i.substr(position, 1) == "(") {
                Token t(GREATER_THAN, '>'); //might break?
                tokens.push_back(t);
                for (int i = 0; i < count; i++) {
                    unadvance();
                }
                advance();
            } else {
                report("Expected >(, recieved >" + i.substr(position, 1));
            }
        } else if (i[position] == 'E') {
            int count = 0;
            advance();
            count++;
            if (i.substr(position, 1) == "(") {
                Token t(EQUAL, 'E');
                tokens.push_back(t);
                for (int i = 0; i < count; i++) {
                    unadvance();
                }
                advance();
            } else {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                advance();
            }
        } else if (i[position] == '*') {
            Token t(COMMENT, i[position]);
            tokens.push_back(t);
            int count = 0;
            advance();
  
            while (count < 15) { //while (i[position] != '*') {
                if (isasciicom(i[position])) {
                    count++;
                    Token t3(LETTER, i[position]);
                    tokens.push_back(t3);
                    position++;
                } else {
                    report("Invalid character in comment, recieved " + i.substr(position, 1));
                }
            }

            if (count != 15) {
                report("Comment is not 15 characters long, recieved " + to_string(count) + " characters");
            } else if (i[position] != '*') {
                report("Comment is not 15 characters long!");
            } else {
                Token t4(COMMENT, '*');
                tokens.push_back(t4);
                advance();
            }
        } else if (i[position] == 'g') { //is followed by an n123
            int count = 0;
            bool valid = false;
            advance();
            count++;
            if (i[position] == 'n') {
                advance();
                count++;
                if (isdigit(i[position])) {
                    valid = true;
                    Token t(INPUT_G, 'g');
                    tokens.push_back(t);
                    advance();
                }
                else {
                    report("Expected g followed by n and a digit, recieved g" + i.substr(position, 1));
                }
            } else {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                continue;
            }
            for (int i = 0; i < count; i++) {
                unadvance();
            }
        } else if (i[position] == 'o') { //is followed by an n123
            int count = 0;
            bool valid = false;
            advance();
            count++;
            if (i[position] == 'n') {
                advance();
                count++;
                if (isdigit(i[position])) {
                    valid = true;
                    Token t(OUTPUT_O, 'o');
                    tokens.push_back(t);
                    advance();
                }
                else {
                    report("Expected o followed by n and a digit, recieved g" + i.substr(position, 1));
                }
            } else {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                continue;
            }
            for (int i = 0; i < count; i++) {
                unadvance();
            }
        } else if (i[position] == 'r') {
            int count = 0;
            bool valid = false;
            advance();
            count++;
            if (i[position] == 's') {
                advance();
                count++;
                if (isdigit(i[position])) {
                    valid = true;
                    Token t(RESPONSE, 'r');
                    tokens.push_back(t);
                    advance();
                }
                else {
                    report("Expected r followed by s and a digit, recieved r" + i.substr(position, 1));
                }
            } else {
                Token t(LETTER, i[position]);
                tokens.push_back(t);
                continue;
            }
            for (int i = 0; i < count; i++) {
                unadvance();
            }
        } else if (i[position] == '\"') {
            Token t(STRING, i[position]);
            tokens.push_back(t);
            int count = 0;
            int trueCount = 0;
            advance();

            while (count < 15) { //while (i[position] != '\"') {
                if (isasciistr(i[position])) {
                    count++;
                    Token t3(LETTER, i[position]);
                    tokens.push_back(t3);
                    position++;
                } else {
                    report("Invalid character in string, recieved " + i.substr(position, 1));
                }
            }

            if (count != 15) {
                report("String is not 15 characters long, recieved " + to_string(count) + " characters");
            } else if (i[position] != '\"') {
                report("String is not 15 characters long!");
            } else {
                Token t4(STRING, '\"');
                tokens.push_back(t4);
                advance();
            }
        } else if (i[position] == 'a' || i[position] == 'b' || i[position] == 'c' || i[position] == 'd' || i[position] == 'e' || i[position] == 'f' 
        || i[position] == 'h' || i[position] == 'i' || i[position] == 'j' || i[position] == 'k' || i[position] == 'l' || i[position] == 'm' 
        || i[position] == 'n' || i[position] == 'p' || i[position] == 'q' || i[position] == 's' || i[position] == 't' || i[position] == 'u' 
        || i[position] == 'v' || i[position] == 'w' || i[position] == 'x' || i[position] == 'y' || i[position] == 'z' || i[position] == 'A'
        || i[position] == 'B' || i[position] == 'C' || i[position] == 'D' || i[position] == 'E' || i[position] == 'F' || i[position] == 'G'
        || i[position] == 'H' || i[position] == 'I' || i[position] == 'J' || i[position] == 'K' || i[position] == 'L' || i[position] == 'M'
        || i[position] == 'N' || i[position] == 'O' || i[position] == 'P' || i[position] == 'Q' || i[position] == 'R' || i[position] == 'S'
        || i[position] == 'T' || i[position] == 'U' || i[position] == 'V' || i[position] == 'W' || i[position] == 'X' || i[position] == 'Y'
        || i[position] == 'Z' || i[position] == 32 ) {
            Token t(LETTER, i[position]);
            tokens.push_back(t);
            advance();
            continue;

        } else if(i[position] == 13 || i[position] == 10) {
            advance();
        } else {
            cout << "character is " << int(i[position]) << endl;
            report("Invalid character, recieved " + i.substr(position, 1));
        }
    }
    return tokens;
}


