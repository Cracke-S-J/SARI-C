// Copyright (c) ssj. All rights reserved.

#define TODO 0

#include "parser.h"
#include "inter.h"
#include "log.h"


void Parser::decls() {
    log_msg("decl start.");
    while(this->look->getTag() == Tags::BASIC) {
        Array* p = (Array*)this->_type();
        Token* tok = this->look;
        this->match(Tags::ID);
        this->match(';');
        Id* _id = new Id((Word*)tok, p, this->used);
        this->top->insert(tok, _id);
        this->used = this->used + p->getWidth();
    }
    log_msg("decl finish.");
    return;
}

Node* Parser::_type() {
    Array* tmp = (Array*)this->look;
    this->match(Tags::BASIC);
    if (this->look->getTag() != '[') {
        return (Node*)tmp;
    }
    else {
        return (Node*)this->dims(tmp);
    }
}

Array* Parser::dims(Array* arr) {
    this->match('[');
    Number* tmp = (Number*)this->look;
    this->match(Tags::NUM);
    this->match(']');
    if (this->look->getTag() == '[') {
        return this->dims(arr);
    }
    else {
        return new Array(tmp->getNumber(), arr);
    }
}

Seq* Parser::block() {
    log_msg("block start.");
    this->match('{');
    Environ* savedEnv = this->top;
    this->top = new Environ(this->top);
    this->decls();
    Seq* s = (Seq*)this->stmts();
    s->setClazz(Inter::SEQ);
    this->match('}');
    this->top = savedEnv;
    log_msg("block finish.");
    return s;
}

Stmt* Parser::stmts() {
    log_msg("stmts start.");
    if (this->look->getTag() == '}') {
        log_msg("stmts end.");
        Stmts.Null->setClazz(Inter::STMT);
        return (Seq*)Stmts.Null;
    }
    else {
        Seq* ret = new Seq((Stmt*)this->stmts(), (Stmt*)this->stmt());
        ret->setClazz(Inter::SEQ);
        return ret;
    }
}

void* Parser::stmt() {
    Stmt* saveStmt = nullptr;
    if (this->look->getTag() == ';') {
        this->move();
        return (Stmt*)Stmts.Null;
    }
    else if (this->look->getTag() == Tags::IF) {
        this->match(Tags::IF);
        this->match('(');
        Expr* x = (Expr*)this->_bool();
        this->match(')');
        Stmt* s1 = (Stmt*)this->stmt();
        if (this->look->getTag() != Tags::ELSE) {
            If* ret = new If(x, s1);
            ret->setClazz(Inter::IF);
            log_msg("if ret");
            return ret;
        }
        this->match(Tags::ELSE);
        Stmt* s2 = (Stmt*)this->stmt();
        Else* ret = new Else(x, s1, s2);
        ret->setClazz(Inter::ELSE);
        return ret;
    }
    else if (this->look->getTag() == Tags::WHILE) {
        While* whilenode = new While();
        whilenode->setClazz(Inter::WHIL);
        Stmt* saveStmt = (Stmt*)Stmts.Enclosing;
        Stmts.Enclosing = (Stmt*)whilenode;
        this->match(Tags::WHILE);
        this->match('(');
        Node* x = this->_bool();
        this->match(')');
        Stmt* s1 = (Stmt*)this->stmt();
        whilenode->init((Expr*)x, s1);
        Stmts.Enclosing = saveStmt;
        return whilenode;
    }
    else if(this->look->getTag() == Tags::DO) {
        Do* donode = new Do();
        donode->setClazz(Inter::DO);
        Stmt* savedStmt = Stmts.Enclosing;
        Stmts.Enclosing = donode;
        this->match(Tags::DO);
        Stmt* s1 = (Stmt*)this->stmt();
        this->match(Tags::WHILE);
        this->match('(');
        Expr* x = (Expr*)this->_bool();
        this->match(')');
        this->match(';');
        donode->init(x, s1);
        Stmts.Enclosing = savedStmt;
        return donode;
    }
    else if(this->look->getTag() == Tags::BREAK) {
        this->match(Tags::BREAK);
        this->match(';');
        Break* _break = new Break();
        _break->setClazz(Inter::BREA);
        return _break;
    }
    else if(this->look->getTag() == '{') {
        return this->block();
    }
    else {
        return this->assign();
    }
}

Set* Parser::assign() {
    log_msg("assign start.");
    Token* t = this->look;
    this->match(Tags::ID);
    Id* id = (Id*)this->top->get(t);
    log_msg("assign id: " + id->toString());
    if(id == nullptr) {
        this->error(t->toString() + " undeclared.");
    }
    Set* set;
    if(this->look->getTag() == '=') {
        this->move();
        set = new Set(id, (Expr*)this->_bool());
        set->setClazz(Inter::SET);
    }
    else {
        Access* x = this->offset(id);
        this->match('=');
        set = (Set*)new SetElem(x, (Expr*)this->_bool());
        set->setClazz(Inter::SETE);
    }
    this->match(';');
    log_msg("assign end.");
    return set;
}

Expr* Parser::_bool() {
    Expr* x = this->join();
    while (this->look->getTag() == Tags::OR) {
        Token* tok = this->look;
        this->move();
        log_msg("new or");
        Or* tmp = new Or((Word*)tok, (Expr*)x, (Expr*)this->join());
        tmp->getExpr1()->setType(Types.Bool);
        tmp->getExpr2()->setType(Types.Bool);
        x = (Expr*)tmp;
        x->setType(Types.Bool);
        x->setClazz(Inter::OR);
        log_msg("new or end");
    }
    return x;
}
Expr* Parser::join() {
    Expr* x = this->equality();
    while (this->look->getTag() == Tags::AND) {
        Token* tok = this->look;
        this->move();
        And* tmp = new And((Word*)tok, (Expr*)x, (Expr*)this->join());
        tmp->getExpr1()->setType(Types.Bool);
        tmp->getExpr2()->setType(Types.Bool);
        x = (Expr*)tmp;
        x->setType(Types.Bool);
        x->setClazz(Inter::AND);
    }
    return x;
}
Expr* Parser::equality() {
    Expr* x = this->rel();
    while (this->look->getTag() == Tags::EQ ||
           this->look->getTag() == Tags::NE) {
        Token* tok = this->look;
        tok->setClazz(Clazz::WORD);
        this->move();
        x = new Rel((Word*)tok, (Arith*)x, this->expr());
        x->setClazz(Inter::REL);
    }
    return x;
}
Expr* Parser::rel() {
    Arith* x = this->expr();
    if (this->look->getTag() == '<' ||
        this->look->getTag() == Tags::LE ||
        this->look->getTag() == Tags::GE ||
        this->look->getTag() == '>') {
        Token* tok = this->look;
        if(tok->getTag() == Tags::LE ||
            tok->getTag() == Tags::GE) {
            log_msg(">= or <=");
            tok->setClazz(Clazz::WORD);
        }
        else {
            tok->setClazz(Clazz::TOKEN);
        }
        this->move();
        Rel* rel = new Rel((Word*)tok, x, this->expr());
        rel->setClazz(Inter::REL);
        return rel;
    }
    return x;
}
Arith* Parser::expr() {
    log_msg("expr start.");
    Arith* arith = (Arith*)this->term();
    while (this->look->getTag() == '+' || 
           this->look->getTag() == '-') {
        Token* tok = this->look;
        this->move();
        arith = new Arith((Word*)tok, arith, this->term());
        arith->setClazz(Inter::ARIT);
    }
    log_msg("expr end.");
    return arith;
}
Arith* Parser::term() {
    Arith* arith = (Arith*)this->unary();
    while (this->look->getTag() == '*' || 
           this->look->getTag() == '/') {
        Token* tok = this->look;
        this->move();
        arith = new Arith((Word*)tok, arith, this->unary());
        arith->setClazz(Inter::ARIT);
    }
    return arith;
}
Expr* Parser::unary() {
    if(this->look->getTag() == '-') {
        this->move();
        return new Unary(Words.minus, this->unary());
    }
    else if(this->look->getTag() == '!') {
        Token* tok = this->look;
        this->move();
        return new Not((Word*)tok, this->unary());
    }
    else {
        return (Expr*)this->factor();
    }
    return nullptr;
}
Expr* Parser::factor() {
    Expr* x = nullptr;
    if(this->look->getTag() == '(') {
        this->move();
        x = (Expr*)this->_bool();
        this->match(')');
    }
    else if(this->look->getTag() == Tags::NUM) {
        x = new Constant(this->look, Types.Int);
        x->getOp()->setClazz(Clazz::NUM);
        x->setClazz(Inter::CONS);
        this->move();
    }
    else if(this->look->getTag() == Tags::REAL) {
        x = new Constant(this->look, Types.Float);
        x->getOp()->setClazz(Clazz::REAL);
        x->setClazz(Inter::CONS);
        this->move();
    }
    else if(this->look->getTag() == Tags::TRUE) {
        x = Constants.True;
        x->getOp()->setClazz(Clazz::WORD);
        Constants.True->setClazz(Inter::CONS);
        this->move();
    }
    else if(this->look->getTag() == Tags::FALSE) {
        x = Constants.False;
        x->getOp()->setClazz(Clazz::WORD);
        Constants.False->setClazz(Inter::CONS);
        this->move();
    }
    else if (this->look->getTag() == Tags::ID) {
        Id* id = (Id*)this->top->get(this->look);
        if(id == nullptr) {
            this->error(this->look->toString() + " undeclared");
        }
        this->move();
        if(this->look->getTag() != '[') {
            id->setClazz(Inter::ID);
            return id;
        }
        else{
            return this->offset(id);
        }
    }
    return x;
}
Access* Parser::offset(Id* id) {
    log_msg("offset start");
    Array* _type = (Array*)id->getType();
    this->match('[');
    Node* i = this->_bool();
    this->match(']');
    _type = _type->getOf();
    log_msg("1");
    Constant* w = new Constant(_type->getWidth());
    Arith* t1   = new Arith((Word*)new Token('*'), (Expr*)i, (Expr*)w);
    Arith* loc  = t1;
    while(this->look->getTag() == '[') {
        this->match('[');
        i = this->_bool();
        this->match(']');
        _type = _type->getOf();
        w = new Constant(_type->getWidth());
        t1 = new Arith((Word*)new Token('*'), (Expr*)i, (Expr*)w);
        Arith* t2 = new Arith((Word*)new Token('+'), (Expr*)loc, (Expr*)t1);
        loc = t2;
    }
    log_msg("offset end");
    return new Access((Array*)id, loc, _type);
}

void Parser::program() {
    Seq* s = this->block();
    Stmt* stmt = (Stmt*)s;
    if(stmt == Stmts.Null) {
        return;
    }
    int begin = s->newlable();
    int after = s->newlable();
    s->emitlabel(begin);
    s->gen(begin, after);
    s->emitlabel(after);
    return;
}
