// Copyright (c) ssj. All rights reserved.

#pragma once

#include "inter.h"
#include "op.h"

// extern STMTS Stmts;

class Break : public Stmt {
private:
    Stmt* stmt;
public:
    Break();
    ~Break(){}
    void gen(int b, int a) {
        this->emit("goto L" + std::to_string(this->stmt->getAfter()));
    }
};

class If : public Stmt {
private:
    Expr* expr;
    Stmt* stmt;
public:
    If(){}
    ~If(){}
    If(Expr* x, Stmt* s) : expr(x), stmt(s) {
        if (!this->expr->getType()->toString().compare("bool")) {
            this->expr->error("boolean required in if");
        }
    }
    void gen(int b, int a) {
        int label = this->newlable();
        this->expr->jumping(0, a);
        this->emitlabel(label);
        this->stmt->gen(label, a);
        return;
    }
};

class Else : public Stmt {
private:
    Expr* expr;
    Stmt* stmt1;
    Stmt* stmt2;
public:
    Else(){}
    ~Else(){}
    Else(Expr* x, Stmt* s1, Stmt* s2) : expr(x), stmt1(s1), stmt2(s2) {
        if (!this->expr->getType()->toString().compare("bool")) {
            this->expr->error("boolean required in if");
        }
    }
    void gen(int b, int a) {
        int label1 = this->newlable();
        int label2 = this->newlable();
        this->expr->jumping(0, label2);
        this->emitlabel(label1);
        this->stmt1->gen(label1, a);
        this->emit("goto L" + std::to_string(a));
        this->emitlabel(label2);
        this->stmt2->gen(label2, a);
    }
};

class Do : public Stmt {
private:
    Expr* expr;
    Stmt* stmt;
public:
    Do() : expr(nullptr), stmt(nullptr) {}
    ~Do(){}
    Do(Expr* x, Stmt* s) : expr(x), stmt(s) {
        if (!this->expr->getType()->toString().compare("bool")) {
            this->expr->error("boolean required in do");
        }
    }
    void init(Expr* x, Stmt* s) {
        expr = x;
        stmt = s;
        if (!this->expr->getType()->toString().compare("bool")) {
            this->expr->error("boolean required in do");
        }
    }
    void gen(int b, int a) {
        this->setAfter(a);
        int label = this->newlable();
        this->stmt->gen(b, label);
        this->emitlabel(label);
        this->expr->jumping(b, 0);
    }
};

class While : public Stmt {
private:
    Expr* expr;
    Stmt* stmt;
public:
    While() : expr(nullptr), stmt(nullptr) {}
    ~While(){}
    While(Expr* x, Stmt* s) : expr(x), stmt(s) {
        if (!this->expr->getType()->toString().compare("bool")) {
            this->expr->error("boolean required in do");
        }
    }
    void init(Expr* x, Stmt* s) {
        expr = x;
        stmt = s;
        if (!this->expr->getType()->toString().compare("bool")) {
            this->expr->error("boolean required in do");
        }
    }
    void gen(int b, int a) {
        this->setAfter(a);
        this->expr->jumping(0, a);
        int label = this->newlable();
        this->emitlabel(label);
        this->stmt->gen(label, a);
        this->emit("goto L" + std::to_string(b));
    }
};

class Set : public Stmt {
private:
    Id* id;
    Expr* expr;
public:
    Set(){}
    ~Set(){}
    Set(Id* i, Expr* x) : id(i), expr(x) {
        if (!this->check(this->id->getType(), this->expr->getType())) {
            this->error("type error");
        }
    }
    Type* check(Type* p1, Type* p2) {
        if (Type::numeric(p1) && Type::numeric(p2)) {
            return p2;
        }
        else if (!p1->getLexeme().compare("bool") && 
                 !p2->getLexeme().compare("bool")) {
            return p2;
        }
        else {
            return nullptr;
        }
    }
    void gen(int b, int a) {
        log_msg(this->id->toString());
        log_msg(this->expr->gen()->toString());
        this->emit(this->id->toString() + " = " + this->expr->gen()->toString());
    }
};

class SetElem : public Stmt {
private:
    Array* array;
    Expr* expr;
    Expr* index;
public:
    SetElem(){}
    ~SetElem(){}
    SetElem(Access* x, Expr* y) : expr(y), array(x->getArray()), index(x->getIndex()) {
        if (!this->check(x->getType(), this->expr->getType())) {
            this->error("type error");
        }
    }
    Type* check(Type* p1, Type* p2) {
        if (!p1->getLexeme().compare(p2->getLexeme())) {
            return p2;
        }
        else if(Type::numeric(p1) && Type::numeric(p2)) {
            return p2;
        }
        else {
            return nullptr;
        }
    }
    void gen(int b, int a) {
        std::string str1 = this->index->reduce()->toString();
        std::string str2 = this->expr->reduce()->toString();
        this->emit(this->array->toString() + "[" + str1 + "] = " + str2);
    }
};

class Seq : public Stmt {
private:
    Stmt* stmt1;
    Stmt* stmt2;
    Set*  set;
    // Seq
public:
    Seq(){}
    ~Seq(){}
    Seq(Stmt* s1, Stmt* s2) : stmt1(s1), stmt2(s2), set(nullptr) {}
    Seq(Stmt* s1, Set* s2) : stmt1(s1), set(s2), stmt2(nullptr) {}
    void gen(int b, int a);
};
