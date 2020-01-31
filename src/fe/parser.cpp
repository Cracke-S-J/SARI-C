// Copyright (c) ssj. All rights reserved.

#define TODO 0

#include "parser.h"

Seq* Parser::stmts() {
    if (this->look->getTag() == (int)"}") {
        return (Seq*)Stmts.Null;
    }
    else {
        return new Seq(this->stmt(), this->stmts());
    }
}

Stmt* Parser::stmt() {

}

Stmt* Parser::assign() {

}
Node* Parser::_bool() {
    Node* x = this->equality();
    while (this->look->getTag() == Tags::OR) {
        Token* tok = this->look;
        this->move();
        x = (Node*)new Or((Word*)tok, (Expr*)x, (Expr*)this->equality());
    }
    return x;
}
Node* Parser::join() {
    Node* x = this->equality();
    while (this->look->getTag() == Tags::AND) {
        Token* tok = this->look;
        this->move();
        x = (Node*)new And((Word*)tok, (Expr*)x, (Expr*)this->equality());
    }
    return x;
}
Node* Parser::equality() {
    Node* x = this->rel();
    while (this->look->getTag() == Tags::EQ ||
        this->look->getTag() == Tags::NE) {
        Token* tok = this->look;
        this->move();
        x = new Rel((Word*)tok, (Arith*)x, this->expr());
    }
    return x;
}
Node* Parser::rel() {
    Arith* x = this->expr();
    if (this->look->getTag() == (int)"<" ||
        this->look->getTag() == Tags::LE ||
        this->look->getTag() == Tags::GE ||
        this->look->getTag() == (int)">") {
        Token* tok = this->look;
        this->move();
        return new Rel((Word*)tok, x, this->expr());
    }
    return x;
}
Arith* Parser::expr() {
    Arith* arith = (Arith*)this->term();
    while (this->look->getTag() == (int)"+" || 
            this->look->getTag() == (int)"-") {
        Token* tok = this->look;
        this->move();
        arith = new Arith((Word*)tok, arith, this->term());
    }
    return arith;
}
Arith* Parser::term() {
    Arith* arith = (Arith*)this->unary();
    while (this->look->getTag() == (int)"*" || 
            this->look->getTag() == (int)"/") {
        Token* tok = this->look;
        this->move();
        arith = new Arith((Word*)tok, arith, this->unary());
    }
    return arith;
}
Logical* Parser::unary() {

}
Node* Parser::factor() {

}
Access* Parser::offset(Id* id) {
    # if TODO
    Array* _type = id->getType();
    this->match((int)"[");
    Logical* i = this->_bool();
    this->match((int)"]");
    _type = _type->of;
    #endif
}

void Parser::program() {
    Stmt* s = this->block();
    int begin = s->newlable();
    int after = s->newlable();
    s->emitlabel(begin);
    s->gen(begin, after);
    s->emitlabel(after);
    return;
}