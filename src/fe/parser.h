// Copyright (c) ssj. All rights reserved.


#include "lexer.h"
#include "symbol.h"
#include "inter.h"

#include <iostream>
#include <string>

class Parser {
private:
    Lexer* lex;
    top = nullptr;
    int used = 0;
    Word* look;
    void move() {
        look = lex->scan();
    }
    void error(std::string str) {
        std::cout << "near line " << std::to_string(lex->getLine()) \
                    << ": " << str << std::endl;
    }
    void match(int t) {
        if (this->look.tag == t) {
            this->move();
        }
        else{
            this->error("syntax error");
        }
    }
    Stmt* block(){}
    void decls(){}
    

public:
    Parser(){}
    Parser(Lexer* lexer) : lex(lexer){
        this->move();
    }
    void program(){}
};