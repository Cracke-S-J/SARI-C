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
        if (!this->expr->getType()->isBool()) {
            this->expr->error("boolean required in if");
        }
    }
    void gen(int b, int a);
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
        if (!this->expr->getType()->isBool()) {
            this->expr->error("boolean required in if");
        }
    }
    void gen(int b, int a);
};

class Do : public Stmt {
private:
    Expr* expr;
    Stmt* stmt;
public:
    Do() : expr(nullptr), stmt(nullptr) {}
    ~Do(){}
    Do(Expr* x, Stmt* s) : expr(x), stmt(s) {
        if (!this->expr->getType()->isBool()) {
            this->expr->error("boolean required in do");
        }
    }
    void init(Expr* x, Stmt* s) {
        expr = x;
        stmt = s;
        if (!this->expr->getType()->isBool()) {
            this->expr->error("boolean required in do");
        }
    }
    void gen(int b, int a);
};

class While : public Stmt {
private:
    Expr* expr;
    Stmt* stmt;
public:
    While() : expr(nullptr), stmt(nullptr) {}
    ~While(){}
    While(Expr* x, Stmt* s) : expr(x), stmt(s) {
        if (!this->expr->getType()->isBool()) {
            this->expr->error("boolean required in while");
        }
    }
    void init(Expr* x, Stmt* s) {
        expr = x;
        stmt = s;
        if (!this->expr->getType()->isBool()) {
            this->expr->error("boolean required in while");
        }
    }
    void gen(int b, int a);
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
            exit(0);
        }
    }
    Type* check(Type* p1, Type* p2) {
        if(!p1 || !p2) {
            return nullptr;
        }
        else if (Type::numeric(p1) && Type::numeric(p2)) {
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
        log_msg("set->gen()" + this->id->toString());
        int expr_t = expr->getClazz();
        if(expr_t == Inter::ARIT) {
            Arith* arith = (Arith*)this->expr;
            // log_msg(arith->gen()->toString());
            this->emit(this->id->toString() + " = " + 
                       arith->gen()->toString());
        }
        else {
            log_msg(this->expr->gen()->toString());
            this->emit(this->id->toString() + " = " + 
                       this->expr->gen()->toString());
        }
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
    Stmt* nxt;
    Stmt* stmt;
public:
    Seq(){}
    ~Seq(){}
    Seq(Stmt* s1, Stmt* s2) : nxt(s1), stmt(s2) {}
    void gen(int b, int a);
};
