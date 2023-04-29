#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include "lexer.h"
#include "parser.h"
#include "node.h"
#include "ast_node.h"

#include "symbol_table.h"
// #include "scope_analyzer.h"


int main() {
    int count = 0;
    bool lexFail = false;
    bool parseSuccess = true;

    ASTNode* finally = nullptr;

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
            finally = ast->buildAndPrintAST(ast);

            std::ofstream xmlFile("output.xml");
            xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            xmlFile << "<?xml-stylesheet type=\"text/xsl\" href=\"stylesheet.xsl\"?>\n";
            // xmlFile << "<Root>" << endl;
            ast->ASTtoXML(finally, xmlFile);
            // xmlFile << "</Root>" << endl;
            xmlFile.close();
            cout << "Parsing succesful and XML generated in output.xml" << endl;
        }
    }
    /* END OF PARSER */
    // finally = ABSTRACT SYNTAX TREE ROOT

    /* START OF SCOPE ANALYSIS */
    cout << endl;
    SymbolTable symbolTable;
    std::stack<int> scopeStack;
    bool scopeSuccess = true;

    if (finally) {
        try {
            symbolTable.traverseAST(finally, 0, scopeStack, symbolTable, AST_PROGR);

            symbolTable.analyzeScopes(finally, 0, scopeStack, symbolTable, AST_PROGR);
            
        } catch (const std::runtime_error& e) {
            std::cerr << "Caught exception: " << e.what() << std::endl << endl;
            scopeSuccess = false;
        } catch (...) {
            std::cerr << "Caught an unknown exception" << std::endl;
            scopeSuccess = false;
        }
    }

    if (scopeSuccess) {
        symbolTable.checkUncalledProcesses();
        cout << "Scope analysis successful" << endl;
        
        symbolTable.checkUncalledProcesses();
        symbolTable.printTable();

        symbolTable.outputTableToHTML();
        cout << endl << "Symbol table output to HTML" << endl;
        cout << "File can be found in the same directory as this program (index.html)" << endl;
    } else {
        cout << "Scope analysis failed" << endl;
    }

}