#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>   
#include <cstdlib> 

#include <regex>
#include <iterator>
#include "lexer.h"
#include "node.h"

using namespace std;

class Parser {
public: 
    Parser(std::vector<Token> tokens);

    Node* parse();
    Node* progr(); 
    Node* procdefs(); 
    Node* proc(); 
    Node* digits(); 
    Node* d(); 
    Node* more(); 
    Node* algo(); 
    Node* seq(); 
    Node* instr(); 
    Node* call(); 
    Node* assign(); 
    Node* loop(); 
    Node* branch(); 
    Node* nodeElse();           //else
    Node* numvar(); 
    Node* boolvar(); 
    Node* stringv(); 
    Node* numexpr(); 
    Node* decnum(); 
    Node* neg();
    Node* pos();
    Node* nodeInt();            //int
    Node* boolexpr();
    Node* logic();
    Node* cmpr();
    Node* stri();
    Node* c();
    Node* comment();
    Node* input();
    Node* output();
    Node* value();
    Node* text();
    Node* terminal();

    void incCurrent();
    void report(string why);
    bool success;

private:
    std::vector<Token> tokens;
    size_t current;
    bool halted;
    bool wow;
    int count;

};