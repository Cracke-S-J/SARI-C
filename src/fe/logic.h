// Copyright (c) ssj. All rights reserved.

#pragma once

#include "inter.h"
#include "op.h"

extern TYPES Types;

class And : public Logical {
public:
    And(){}
    ~And(){}
    And(Word* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {}
    void jumping(int t, int f);
};

class Or : public Logical {
public:
    Or(){}
    ~Or(){}
    Or(Word* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {}
    void jumping(int t, int f);
};

class Not : public Logical {
public:
    Not(){}
    ~Not(){}
    Not(Word* tok, Expr* x) : Logical(tok, x, x) {}
    void jumping(int t, int f) {
        this->expr2->jumping(f, t);
    }
    std::string toString() {
        return this->op->toString() + " " + this->expr2->toString();
    }
};

class Rel : public Logical {
public:
    Rel(){}
    ~Rel(){}
    Rel(Word* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {
        log_msg("new rel");
        this->type = this->check(x1->getType(), x2->getType());
        if (this->type == nullptr) {
            // std::cout << "type error" << std::endl;
            // exit(0);
        }
        log_msg("new rel end");
    }
    Type* check(Type* p1, Type* p2);
    void Rel::jumping(int t, int f);
};
