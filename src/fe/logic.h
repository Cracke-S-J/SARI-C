// Copyright (c) ssj. All rights reserved.

#pragma once

#include "inter.h"

class And : public Logical {
public:
    And(){}
    ~And(){}
    And(Word* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {}
    void jumping(int t, int f) {
        int label = f ? f : this->newlabel();
        this->expr1->jumping(0, f);
        this->expr2->jumping(0, f);
        if (f == 0)
            this->emitlabel(label);
    }
};

class Or : public Logical {
public:
    Or(){}
    ~Or(){}
    Or(Word* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {}
    void jumping(int t, int f) {
        int label = t ? t : this->newlabel();
        this->expr1->jumping(label, 0);
        this->expr2->jumping(t, f);
        if (t == 0)
            this->emitlabel(label);
    }
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
    Rel(Word* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {}
    Type* check(Type* p1, Type* p2) {
        if (p1 == p2) {
            return Types.Bool;
        }
        else {
            return nullptr;
        }
    }
    void jumping(int t, int f) {
        Expr* a = this->expr1->reduce();
        Expr* b = this->expr2->reduce();
        std::string str = a->toString() + " " + this->op->toString() + b->toString();
        this->emitjumps(str, t, f);
        return;
    }
};
