#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "node.h"
#include "ast_node.h"

#include "symbol_table.h"
#include "scope_analyzer.h"


int main() {
    int count = 0;
    bool lexFail = false;
    bool parseSuccess = true;

    /* START OF LEXER */
    std::ifstream inFile("*.txt");
    std::string input((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    cout << "Input program read... starting lexical analysis" << endl;
    Lexer lexer(input);
    std::vector<Token> tokens;
    try {
        tokens = lexer.tokenize(input);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        lexFail = true;
    }

    if (!lexFail) {
        cout << "Tokens read:" << endl;
        for ( Token& token : tokens) {
            std::cout << count++ << " ";
            std::cout << "Token: " << token.value << "\tType: " << token.getType() << std::endl;
        }
        cout << endl;
    }
    /* END OF LEXER */

    /* START OF PARSER */
    if (!lexFail) {
        Parser parser(tokens);
        Node* ast;
        try {
            ast = parser.parse();
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Caught exception: " << e.what() << std::endl << endl;
            parseSuccess = false;
        } catch (...) {
            std::cerr << "Caught an unknown exception" << std::endl;
            parseSuccess = false;
        }

        if (parseSuccess) {
            // ast->printAST(ast);
            ASTNode* finally = ast->buildAndPrintAST(ast);

            std::ofstream xmlFile("output.xml");
            xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            xmlFile << "<?xml-stylesheet type=\"text/xsl\" href=\"stylesheet.xsl\"?>\n";
            // xmlFile << "<Root>" << endl;
            ast->ASTtoXML(finally, xmlFile);
            // xmlFile << "</Root>" << endl;
            xmlFile.close();
            
        }
    }
    /* END OF PARSER */
    // finally = ABSTRACT SYNTAX TREE ROOT

    SymbolTable symbol_table;




}