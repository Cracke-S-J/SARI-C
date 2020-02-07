// Copyright (c) ssj. All rights reserved.

#pragma once

#include "inter.h"

extern TYPES Types;

class And : public Logical {
public:
    And(){}
    ~And(){}
    And(Word* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {}
    void jumping(int t, int f) {
        int label = f ? f : this->newlable();
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
        int label = t ? t : this->newlable();
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
    Rel(Word* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {
        log_msg("new rel");
        this->type = this->check(x1->getType(), x2->getType());
        if (this->type == nullptr) {
            std::cout << "type error" << std::endl;
            // exit(0);
        }
        log_msg("new rel end");
    }
    Type* check(Type* p1, Type* p2);
    void jumping(int t, int f) {
        int expr1_t = this->expr1->getClazz();
        int expr2_t = this->expr2->getClazz();
        int op_t    = this->op->getClazz();
        std::string str1, str2, strop;
        if(expr1_t == Inter::CONS) {
            log_msg("constant->reduce->tostr");
            Constant* cons = (Constant*)expr1;
            str1 = cons->reduce()->toString();
        }
        else {
            str1 = expr1->reduce()->toString();
        }
        if(expr2_t == Inter::CONS) {
            log_msg("constant->reduce->tostr");
            Constant* cons = (Constant*)expr2;
            str2 = cons->reduce()->toString();
        }
        else {
            str2 = expr2->reduce()->toString();
        }
        if(op_t == Clazz::WORD) {
            Word* word = (Word*)op;
            strop = word->toString();
        }
        else {
            strop = this->op->toString();
        }
        this->emitjumps(str1 + strop + str2, t, f);
        return;
    }
};
