// Copyright (c) ssj. All rights reserved.

#pragma once

#include "lexer.h"
#include "symbol.h"
#include "inter.h"
#include "op.h"
#include "logic.h"
#include "stmt.h"
#include "log.h"

#include <iostream>
#include <string>
#include <sstream>

class Parser {
private:
    Lexer* lex;
    Environ* top = nullptr;
    int used = 0;
    Token* look;
    void move() {
        look = (Token*)lex->scan();
        if(look->getTag() < 256) {
            std::stringstream sstr;
            sstr << (char)look->getTag();
            log_msg("look->tag = " + sstr.str());
        }
        else {
            log_msg("look->tag = " + 
                std::to_string(look->getTag()));
        }
        
    }
    void error(std::string str) {
        std::cout << "near line " << std::to_string(lex->line) \
                  << ": " << str << std::endl;
    }
    void match(int t) {
        if(this->look->getTag() == t) {
            this->move();
        }
        else {
            this->error("syntax error");
        }
    }
    Node* _type();
    Array* dims(Array*);
    Seq* block();
    void decls();
    Stmt* stmts();
    void* stmt();
    Set* assign();
    Node* _bool();
    Node* join();
    Node* equality();
    Node* rel();
    Arith* expr();
    Arith* term();
    Expr* unary();
    Expr* factor();
    Access* offset(Id*);

public:
    Parser(){}
    Parser(Lexer* lexer) : lex(lexer), used(0), top(nullptr) {
        this->move();
    }
    ~Parser(){
        #if TODO
        #endif
    }
    void program();
};