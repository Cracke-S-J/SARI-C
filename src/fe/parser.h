// Copyright (c) ssj. All rights reserved.


#include "lexer.h"
#include "symbol.h"
#include "inter.h"
#include "op.h"
#include "logic.h"
#include "stmt.h"

#include <iostream>
#include <string>

class Parser {
private:
    Lexer* lex;
    Environ* top = nullptr;
    int used = 0;
    Token* look;
    void move() {
        look = (Token*)lex->scan();
    }
    void error(std::string str) {
        std::cout << "near line " << std::to_string(lex->line) \
                    << ": " << str << std::endl;
    }
    void match(int t) {
        if (this->look->getTag() == t) {
            this->move();
        }
        else{
            this->error("syntax error");
        }
    }
    Array* _type() {
        Array* tmp = (Array*)this->look;
        this->match(Tags::BASIC);
        if (this->look->getTag() != (int)"[") {
            return tmp;
        }
        else {
            return this->dims(tmp);
        }
    }
    Array* dims(Array* arr) {
        this->match((int)"[");
        Number* tmp = (Number*)this->look;
        this->match(Tags::NUM);
        this->match((int)"]");
        if (this->look->getTag() == (int)"[") {
            return this->dims(arr);
        }
        else {
            return new Array(tmp->getNumber(), arr);
        }
    }
    Stmt* block(){
        this->match((int)"{");
        Environ* savedEnv = this->top;
        this->top = new Environ(this->top);
        this->decls();
        Stmt* s = this->stmts();
        this->match((int)"}");
        this->top = savedEnv;
    }
    void decls(){
        while(this->look->getTag() == Tags::BASIC) {
            Array* p = this->_type();
            Token* tok = this->look;
            this->match(Tags::ID);
            this->match((int)";");
            Id* _id = new Id((Word*)tok, p, this->used);
            this->top->insert(tok, _id);
            this->used = this->used + p->getWidth();
        }
    }
    Seq* stmts();
    Stmt* stmt();
    Stmt* assign();
    Node* _bool();
    Node* join();
    Node* equality();
    Node* rel();
    Arith* expr();
    Arith* term();
    Logical* unary();
    Node* factor();
    Access* offset(Id*);

public:
    Parser(){}
    Parser(Lexer* lexer) : lex(lexer), used(0), top(nullptr) {
        this->move();
    }
    ~Parser(){}
    void program();
};