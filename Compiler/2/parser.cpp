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
#include "parser.h"

using namespace std;

Parser :: Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
    this->current = 0;
    this->halted = false;
    this->success = false;
    this->wow = false;
    this->count = 0;
}

void Parser :: incCurrent() {
    // cout << "Increasing current... " << current << endl;
    // cout << "Current token is: " << tokens[current].getValue() << " and type is: " << tokens[current].toString(tokens[current].type) << endl;
    current++;
}

void Parser :: report(string why) {
    throw std::runtime_error("Parsing error: " + why);
}

Node* Parser :: parse() {
    cout << "Starting construction of parse tree... with progr()" << endl;
    cout << "..." << endl;
    Node* prog = progr();
    cout << "Finished progr() - parsing complete" << endl << "Current is " << current << " and tokens.size() is " << tokens.size() << endl << endl;

    if (halted && current == tokens.size()) {
        cout << "Parser finished executing at position " << current << endl;
        cout << "No parse (syntax) errors found :)" << endl;
        prog->success = true;
        return prog;
    }

    if (current != tokens.size()) {
        string error = "Parsing error! Unexpected token. Token number: " + to_string(current) + " and value: " + tokens[current].value;
        report(error);
    }
    
    if (halted) {
        report("Parser finished executing at position " + to_string(current)  + " and value: " + tokens[current].value);
        cout << "PARSING ERROR" << endl;
        return prog;
    }

    return prog;
}

Node* Parser :: progr() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    Node* node = new Node(PROGR);

    if (tokens[current].type == COMMA) {
        halted = true;
        report("Unexpected comma, program (progr) must start with an ALGO");
    } else {
        node->children.push_back(algo());
        node->children.push_back(procdefs());
    }
    
    return node;
}

Node* Parser::procdefs() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    Node* node = new Node(PROCDEFS);

    if (tokens[current].type == COMMA) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));;
        incCurrent();
        node->children.push_back(proc());
        node->children.push_back(procdefs());
    }    

    return node;
}

Node* Parser::proc() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    Node* node = new Node(PROC);


    if (tokens[current].type == PROCESS) {

        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));;
        incCurrent(); //move passed the p to DIGITS
        node->children.push_back(digits());

        if (tokens[current].type == LEFT_BRACE) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));;
            incCurrent();
            node->children.push_back(progr());
        } else {
            report("PROC must begin with a '{'");
        }
        //after progr processes, we should be at the end of the proc
        if (tokens[current].type == RIGHT_BRACE) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));;
            incCurrent();
        } else {
            report("PROC must end with a '}'");
        }
    } else {
        report("PROC must begin with a 'p' followed by DIGITS");
    }
    
    return node;
}

Node* Parser::digits() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    Node* node = new Node(DIGITS);

    node->children.push_back(d());
    node->children.push_back(more());

    return node;
}

Node* Parser::d() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;

    if (tokens[current].type == DIGIT) {
        Node* node = new Node(TERMINAL, tokens[current].getValue());
        incCurrent();
        return node;
    } else {
        report("Expected a DIGIT");
    }
}

Node* Parser::more() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    Node* node = new Node(MORE);

    if (tokens[current].type == DIGIT) {
        node->children.push_back(digits());
    } else {
        return nullptr; //epsilon
    }

    return node;
}

Node* Parser::algo() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
   
    Node* node = new Node(ALGO);
    node->children.push_back(instr());
    node->children.push_back(comment());
    node->children.push_back(seq());

    return node;
}

Node* Parser::instr() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    Node* node = new Node(INSTR);

    // cout << tokens[current].getValue() << endl;

    switch (tokens[current].type) {
        case INPUT_G:
            node->children.push_back(input());
            break;

        case OUTPUT_O:
            node->children.push_back(output());
            break;

        case NUMERIC_VAR:
            node->children.push_back(assign());
            break;

        case STRINGVAR:
            node->children.push_back(assign());
            break;

        case BOOLVAR:
            node->children.push_back(assign());
            break;

        case CALL:
            node->children.push_back(call());
            break;

        case WHILE:
            node->children.push_back(loop());
            break;

        case IF:
            node->children.push_back(branch());
            break;

        case HALT:
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent(); 
            break;

        case RESPONSE: 
            node->children.push_back(output());
            break;

        default:
            report("Expected input/output/numvar/stringvar/boolvar/call/while/if/halt/response, recieved " + tokens[current].getValue());
    }
    return node;
}

Node* Parser::comment() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(nCOMMENT);

    if (tokens[current].type == COMMENT) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent(); //move past the * to the characters
        node->children.push_back(c());
        if (tokens[current].type == COMMENT) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
        } else {
            report("Expected a comment demarcator '*' to end the comment");
        }
    } else {
        return nullptr; //epsilon
    }


    return node;
}

Node* Parser::seq() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    bool currChange = false;
    Node* node = new Node(SEQ);

    if (tokens[current].type == SEMICOLON) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        node->children.push_back(algo());
    } else {
        return nullptr; //epsilon
    }
    
    return node;
}

Node* Parser::call() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    Node* node = new Node(nCALL);

    if (tokens[current].type == CALL) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        if (tokens[current].type == PROCESS) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            node->children.push_back(digits());
        } else {
            report("Expected a process token 'p' after 'c', recieved " + tokens[current].getValue());
        }
    } else {
        report("Expected a call token 'c', recieved " + tokens[current].getValue());
    }

    return node;
}



Node* Parser::assign() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    Node* node = new Node(ASSIGN);
    
    if (tokens[current].type == NUMERIC_VAR) {
        node->children.push_back(numvar());
        if (tokens[current].type == ASSIGN_OP) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            // cout << endl << endl << endl << tokens[current].getValue() << endl << endl << endl;
            node->children.push_back(numexpr());
        } else {
            report("Expected an assignment operator := after a numeric variable, recieved " + tokens[current].getValue());
        }
    } else if (tokens[current].type == BOOLVAR) {
        node->children.push_back(boolvar());
        if (tokens[current].type == ASSIGN_OP) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            node->children.push_back(boolexpr());

        } else {
            report("Expected an assignment operator := after a boolean variable, recieved " + tokens[current].getValue());
        }
    } else if (tokens[current].type == STRINGVAR) {
        node->children.push_back(stringv());
        if (tokens[current].type == ASSIGN_OP) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            node->children.push_back(stri());
        } else {
            report("Expected an assignment operator := after a string variable, recieved " + tokens[current].getValue());
        }
    } else {
        report("Expected a numeric variable, boolean variable, or string variable, recieved " + tokens[current].getValue());
    }

    return node;
}


Node* Parser::loop() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    Node* node = new Node(LOOP);

    if (tokens[current].type == WHILE) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();

        if (tokens[current].type == LEFT_PAREN) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            // cout << "boolexpr " << tokens[current-1].getValue() << endl;
            node->children.push_back(boolexpr());


            if (tokens[current].type == RIGHT_PAREN) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();

                if (tokens[current].type == LEFT_BRACE) {
                    node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                    incCurrent();
                    node->children.push_back(algo());

                    if (tokens[current].type == RIGHT_BRACE) {
                        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));

                        incCurrent();
                    } else {
                        report("Expected a right brace '}' to end the while loop, recieved " + tokens[current].getValue());
                    }
                } else {
                    report("Expected a left brace '{' to start the while loop, recieved " + tokens[current].getValue());
                }
            } else {
                report("Expected a right parenthesis ')' to end the while loop condition, recieved " + tokens[current].getValue());
            }
        } else {
            report("Expected a left parenthesis '(' to start the while loop condition, recieved " + tokens[current].getValue());
        }
    } else {
        report("Expected a while token 'w', recieved " + tokens[current].getValue());
    }

    return node;
}


Node* Parser::branch() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) return nullptr;
    Node* node = new Node(BRANCH);

    if (tokens[current].type == IF) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();

        if (tokens[current].type == LEFT_PAREN) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            node->children.push_back(boolexpr());
            
            /* QUICK FIX */
            if (tokens[current].type == THEN) {
                current--;
            }
            /* QUICK FIX */
            /* it jumps ahead one extra character sometimes for some reason */

            if (tokens[current].type == RIGHT_PAREN) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();

                if (tokens[current].type == THEN) {
                    node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                    incCurrent();

                    if (tokens[current].type == LEFT_BRACE) {
                        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                        incCurrent();
                    
                        node->children.push_back(algo());

                        if (tokens[current].type == RIGHT_BRACE) {
                            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                            incCurrent();
                            node->children.push_back(nodeElse());
                        } else {
                            report("Expected a right brace '}' to end the if statement, recieved " + tokens[current].getValue());
                        }
                    } else {
                        report("Expected a left brace '{' to start the if statement, recieved " + tokens[current].getValue());
                    }
                } else {
                    report("Expected a then token 't' after the if condition, recieved " + tokens[current].getValue());
                }
            } else {
                report("Expected a right parenthesis ')' to end the if condition, recieved " + tokens[current].getValue());
            }
        } else {
            report("Expected a left parenthesis '(' to start the if condition, recieved " + tokens[current].getValue());
        }
    } else {
        report("Expected an if token 'i', recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::nodeElse() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(nELSE);

    if (tokens[current].type == ELSE) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();

        if (tokens[current].type == LEFT_BRACE) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            node->children.push_back(algo());

            if (tokens[current].type == RIGHT_BRACE) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();
                return node;
            } else {
                report("Expected a right brace '}' to end the else statement, recieved " + tokens[current].getValue());
            }
        } else {
            report("Expected a left brace '{' to start the else statement, recieved " + tokens[current].getValue());
        }
    } else {
        return nullptr;// If current token is not ELSE, the function returns an empty ELSE node (epsilon)
    }

    return node;
}


Node* Parser::numvar() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(NUMVAR);

    if (tokens[current].type == NUMERIC_VAR) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        if (tokens[current].type == DIGIT) {
            node->children.push_back(digits());
        } else {
            report("Expected a digit after a numeric variable, recieved " + tokens[current].getValue());
        }
    } else {
        report("Expected a numeric variable, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::boolvar() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(nBOOLVAR);

    if (tokens[current].type == BOOLVAR) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        if (tokens[current].type == DIGIT) {
            node->children.push_back(digits());
        } else {
            report("Expected a digit after a boolean variable, recieved " + tokens[current].getValue());
        }
    } else {
        report("Expected a boolean variable, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::stringv() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(STRINGV);

    if (tokens[current].type == STRINGVAR) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        if (tokens[current].type == DIGIT) {
            node->children.push_back(digits());
            return node;
        } else {
            report("Expected a digit after a string variable, recieved " + tokens[current].getValue());
        }
    } else {
        report("Expected a string variable, recieved " + tokens[current].getValue());
    }
    return node;
}


Node* Parser::numexpr() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(NUMEXPR);

    cout << current << endl;
    
    if (tokens[current].type == ADD_OP || tokens[current].type == MULTI_OP || tokens[current].type == DIV_OP) {
        string opType = tokens[current].getValue();
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();

        if (tokens[current].type == LEFT_PAREN) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();

            node->children.push_back(numexpr());

            if (tokens[current].type == COMMA) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();
                node->children.push_back(numexpr());

                if (tokens[current].type == RIGHT_PAREN) {
                    node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                    incCurrent();
                    node->value = opType; // Store the operation type in the node data
                } else {
                    report("Expected a right parenthesis ')' to end the " + opType + " operation, recieved " + tokens[current].getValue());
                }
            } else {
                report("Expected a comma ',' to separate the operands of the " + opType + " operation, recieved " + tokens[current].getValue());
            }
        } else {
            report("Expected a left parenthesis '(' to start the " + opType + " operation, recieved " + tokens[current].getValue());
        }
    } else if (tokens[current].type == NUMERIC_VAR) {
        node->children.push_back(numvar());
    } else if (tokens[current].type == DECIMAL) {
        node->children.push_back(decnum());
    } else if (tokens[current].type == MINUS) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        node->children.push_back(pos());
    } else if (tokens[current].type == DIGIT) {
        node->children.push_back(pos());
    } else {
        report("Expected a number expression, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::decnum() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(DECNUM);

    if (tokens[current].type == DECIMAL) {
        node->children.push_back(new Node(TERMINAL, "0.00"));
        incCurrent();
        return node;
    } else if (tokens[current].type == MINUS) {
        node->children.push_back(neg());
    } else if (tokens[current].type == DIGIT) {
        node->children.push_back(pos());
    } else {
        report("Expected a decimal number, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::neg() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(NEG);

    if (tokens[current].type == MINUS) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();

        node->children.push_back(pos());
    } else {
        report("Expected a negative sign '-' to start a negative number, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::pos() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(POS);
    int count = 0;

    node->children.push_back(nodeInt());

    // cout << "Current token: " << tokens[current].getType() << endl; // DEBUG
    if (tokens[current].type == DOT) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        if (tokens[current].type == DIGIT) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));;
            count++;
            incCurrent();
            if (tokens[current].type == DIGIT) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));;
                count++;
                incCurrent();
            } else {
                report("Expected a digit (0-9) after a decimal point aand digit, recieved " + tokens[current].getValue());
            }
        } else {
            report("Expected a digit (0-9) after a decimal point, recieved " + tokens[current].getValue());
        }
    } else {
        // cout << "current: " << current << endl;
        report ("Expected a decimal point '.' to start a decimal number, recieved " + tokens[current].getValue());
    }

    if (count > 2) {
        report("Expected a maximum of two digits after a decimal point, recieved " + to_string(count));
    }

    return node;
}

Node* Parser::nodeInt() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(INT);

    if (tokens[current].type == DIGIT) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        if (tokens[current].type == DIGIT) {
            node->children.push_back(more());
        }
    } else {
        report("Expected a digit (1-9) to start an integer, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::boolexpr() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(BOOLEXPR);

    if (tokens[current].type == BOOLVAR || tokens[current].type == TRUE || tokens[current].type == FALSE ||
        tokens[current].type == AND || tokens[current].type == OR || tokens[current].type == NOT) {
        node->children.push_back(logic());
    } else if (tokens[current].type == EQUAL || tokens[current].type == LESS_THAN || tokens[current].type == GREATER_THAN){
        node->children.push_back(cmpr());
    } else {
        report("Expected a boolean expression, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::logic() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(LOGIC);
    string opType = "logical";


    switch (tokens[current].type) {
        case BOOLVAR:
            node->children.push_back(boolvar());
            break;
        case TRUE:
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            break;
        case FALSE:
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));;
            incCurrent();
            break;
        case AND:
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            if (tokens[current].type == LEFT_PAREN) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();
                node->children.push_back(boolexpr());

                if (tokens[current].type == COMMA) {
                    node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                    incCurrent();

                    node->children.push_back(boolexpr());
                } else {
                    report("Expected a comma after the first boolean expression in an AND statement, recieved " + tokens[current].getValue());
                }

                if (tokens[current].type == RIGHT_PAREN) {
                    node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                    incCurrent();
                } else {
                    report("Expected a right parenthesis to end an AND statement, recieved " + tokens[current].getValue());
                }
            } else {
                report("Expected a left parenthesis to start an AND statement, recieved " + tokens[current].getValue());
            }
            break;

        case OR:
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            if (tokens[current].type == LEFT_PAREN) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();
                node->children.push_back(boolexpr());

                if (tokens[current].type == COMMA) {
                    node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                    incCurrent();

                    node->children.push_back(boolexpr());
                } else {
                    report("Expected a comma after the first boolean expression in an OR statement, recieved " + tokens[current].getValue());
                }

                if (tokens[current].type == RIGHT_PAREN) {
                    node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                    incCurrent();
                } else {
                    report("Expected a right parenthesis to end an OR statement, recieved " + tokens[current].getValue());
                }
            } else {
                report("Expected a left parenthesis to start an OR statement, recieved " + tokens[current].getValue());
            }
            break;

        case NOT: {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();

            if (tokens[current].type == LEFT_PAREN) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();
                node->children.push_back(boolexpr());

                if (tokens[current].type == RIGHT_PAREN) {
                    node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                    incCurrent();
                } else {
                    report("Expected a right parenthesis to end a NOT statement, recieved " + tokens[current].getValue());
                }
            } else {
                report("Expected a left parenthesis to start a NOT statement, recieved " + tokens[current].getValue());
            }
            break;
        }
        default:
            report("Expected a boolean expression, recieved " + tokens[current].getValue());
    }
  
    return node;
}


Node* Parser::cmpr() {//
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(CMPR);

    if (tokens[current].type == EQUAL || tokens[current].type == LESS_THAN || tokens[current].type == GREATER_THAN) {

        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();

        if (tokens[current].type == LEFT_PAREN) {
            node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
            incCurrent();
            node->children.push_back(numexpr());


            if (tokens[current].type == COMMA) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();
                node->children.push_back(numexpr());

            } else {
                report("Expected a comma after the first number expression in a comparison statement, recieved " + tokens[current].getValue());
            }

            if (tokens[current].type == RIGHT_PAREN) {

                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();

                // if (tokens[current].type == RIGHT_PAREN) {
                //     while (tokens[current].type == RIGHT_PAREN) {
                //         incCurrent(); //weird solution but works...
                //     }
                // }

            } else {
                report("Expected a right parenthesis to end a comparison statement, recieved " + tokens[current].getValue());
            }
        } else {
            report("Expected a left parenthesis to start a comparison statement, recieved " + tokens[current].getValue());
        }
    } else {
        report("Expected a comparison statement, recieved " + tokens[current].getValue());
    }

    return node;
}


Node* Parser::stri() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(STRI);
    
    //STRING is actually a QUOTE character "
    if (tokens[current].type == STRING) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent(); // Move past the first " token

        if (tokens[current].type == LETTER) {
            while (tokens[current].type != STRING && tokens[current].type == LETTER) {
                node->children.push_back(c());
            }

            if (tokens[current].type == STRING) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent(); // Move past the second " token
            } else {
                report("Expected a /"", recieved " + tokens[current].getValue());
            }
        }
    } else {
        report("Expected a string, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::c() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(C);

    if (tokens[current].type == LETTER) {
        while (tokens[current].type == LETTER) {
            if (tokens[current+1].type == COMMENT) {
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();
                return node;
            } else if (tokens[current+1].type != COMMENT){
                node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
                incCurrent();
            } 
        }
    } else {
        report("Expected an ascii character (32-127), recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::input() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(INPUT);

    if (tokens[current].type == INPUT_G) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        node->children.push_back(numvar());
    } else {
        report("Expected an input statement, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::output() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(OUTPUT);

    if (tokens[current].type == RESPONSE) {
        node->children.push_back(text());
    } else if (tokens[current].type == OUTPUT_O) {
        node->children.push_back(value());
    } else {
        report("Expected an output/recieve statement, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::value() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(VALUE);

    if (tokens[current].type == OUTPUT_O) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        node->children.push_back(numvar());
    } else {
        report("Expected an output statement, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::text() {
    if (current == tokens.size()) {this->halted = true;}
    if (halted) {return nullptr;}
    Node* node = new Node(TEXT);

    if (tokens[current].type == RESPONSE) {
        node->children.push_back(new Node(TERMINAL, tokens[current].getValue()));
        incCurrent();
        node->children.push_back(stringv());
    } else {
        report("Expected a response statement, recieved " + tokens[current].getValue());
    }

    return node;
}

Node* Parser::terminal() {
    return nullptr;
}










