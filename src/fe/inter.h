// Copyright (c) ssj. All rights reserved.

#pragma once

#include "lexer.h"
#include "symbol.h"

#include <iostream>

class Node {
protected:
    int labels;
    int lexline;
public:
    Node() : labels(0), lexline(Lexer::line) {}
    int newlable() {
        return this->labels++;
    }
    void emitlabel(int idx) {
        std::cout << "L" << std::to_string(idx) << ":";
    }
    void emit(std::string str) {
        std::cout << "\t" << str << std::endl;
    }
    void error(std::string str) {
        std::cout << "near line " << this->lexline << str << std::endl;
    }
};

class Stmt : public Node {
private:
    int after;
public:
    Stmt() : after(0) {}
    void gen(int before, int after) {}
    int getAfter() const{
        return after;
    }
    void setAfter(int a) {
        this->after = a;
    }
};

class STMTS {
public:
    Stmt* Null = new Stmt();
    Stmt* Enclosing = Null;
};

class Expr : public Node {
protected:
    Token* op;
    Type* type;
public:
    Expr(){}
    Expr(Token* tok, Type* _type) : op(tok), type(_type) {}
    Type* getType() const{
        return this->type;
    }
    Token* getOp() const{
        return this->op;
    }
    Expr* gen() {
        return this;
    }
    Expr* reduce() {
        return this;
    }
    std::string toString() const{
        if(this->op->getClazz() == Clazz::NUM) {
            Number* n = (Number*)this->op;
            return n->toString();
        }
        else if(this->op->getClazz() == Clazz::WORD) {
            Word* w = (Word*)this->op;
            return w->toString();
        }
        Word* w = (Word*)this->op;
        return w->toString();
    }
    void emitjumps(std::string str, int t, int f) {
        if (t != 0 && f != 0) {
            this->emit("if " + str + " goto L" + std::to_string(t));
            emit("goto L" + std::to_string(f));
        }
        else if (t != 0) {
            this->emit("if " + str + " goto L" + std::to_string(t));
        }
        else if (f != 0) {
            this->emit("iffalse" + str + " goto L" + std::to_string(t));
        }
    }
    void jumping(int t, int f) {
        this->emitjumps(this->toString(), t, f);
    }
};

class Temp : public Expr {
private:
    int number;
public:
    static int count;
    Temp(){}
    Temp(Type* _type);
    std::string toString() const{
        return "t" + std::to_string(number);
    }
};

class Logical : public Expr {
protected:
    Expr* expr1;
    Expr* expr2;
    Type* type;
    Type* check(Type* a, Type* b) {
        if (!a->getLexeme().compare("bool") &&
            !b->getLexeme().compare("bool")) {
            return new Type("bool", Tags::BASIC, 1);
        }
        else{
            return nullptr;
        }
    }
public:
    Logical(){}
    Logical(Word* tok, Expr* x1, Expr* x2) : Expr(tok, nullptr) {
        this->expr1 = x1;
        this->expr2 = x2;
        this->type  = this->check(x1->getType(), x2->getType());
        if (this->type == nullptr) {
            std::cout << "type error" << std::endl;
            exit(0);
        }
    }
    std::string toString() const{
        return this->expr1->toString() + " " + this->op->toString() + " " + this->expr2->toString();
    }
    Temp* gen() {
        int f = this->newlable();
        int a = this->newlable();
        Temp* tmp = new Temp(this->type);
        this->jumping(0, f);
        this->jumping(0, f);
        this->emit(tmp->toString() + " = true");
        this->emit("goto L" + a);
        this->emitlabel(f);
        this->emit(tmp->toString() + " = false");
        this->emitlabel(a);
        return tmp;
    }
};

class Op : public Expr {
public:
    Op(){}
    ~Op(){}
    Op(Token* op, Type* type) : Expr(op, type) {}
    Temp* reduce() {
        Expr* x = this->gen();
        Temp* t = new Temp(this->type);
        this->emit(t->toString() + " = " + x->toString());
        return t;
    }
};

class Id : public Expr {
private:
    int offset;
public:
    Id(){}
    ~Id(){}
    Id(Word* tok, Type* type, int b) : offset(b), Expr(tok, type) {
        this->getOp()->setClazz(Clazz::WORD);
    }
};

class Unary : public Expr {
private:
    Expr* expr;
    Type* type;
public:
    Unary(){}
    ~Unary(){}
    Unary(Token* tok, Expr* x);
    Unary* gen() {
        return new Unary(this->op, this->expr->reduce());
    }
    std::string toString() {
        return this->op->toString() + " " + this->expr->toString();
    }
};

class Constant : public Expr {
public:
    Constant(){}
    ~Constant(){}
    Constant(int tok, Type* type=nullptr) : Expr((Word*)new Number(tok), type) {}
    Constant(Token* tok, Type* type=nullptr) : Expr(tok, type) {}
    void jumping(int t, int f) {
        if (!t && !this->getOp()->toString().compare("true")) {
            this->emit("goto L" + std::to_string(t));
        }
        else if (!f && !this->getOp()->toString().compare("false")) {
            this->emit("goto L" + std::to_string(f));
        }
    }
};

class CONSTANTS {
public:
    CONSTANTS();
    Constant* True;
    Constant* False;
};

extern TYPES Types;
extern WORDS Words;
extern STMTS Stmts;
extern CONSTANTS Constants;
