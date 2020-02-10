// Copyright (c) ssj. All rights reserved.

#include "symbol.h"
#include "logic.h"

Type* Rel::check(Type* p1, Type* p2) {
    if (p1 == p2) {
        return Types.Bool;
    }
    else {
        return nullptr;
    }
}

void And::jumping(int t, int f) {
    int label = f ? f : this->newlable();
    int expr1_t = this->expr1->getClazz();
    int expr2_t = this->expr2->getClazz();
    if(expr1_t == Inter::AND) {
        And* and = (And*)expr1;
        and->jumping(0, f);
    }
    else if(expr1_t == Inter::REL) {
        Rel* rel = (Rel*)expr1;
        rel->jumping(0, f);
    }
    else if(expr1_t == Inter::CONS) {
        Constant* cons = (Constant*)expr1;
        cons->jumping(0, f);
    }
    else if(expr1_t == Inter::ID) {
        Id* id = (Id*)expr1;
        id->jumping(0, f);
    }
    else {
        this->expr1->jumping(0, f);
    }
    if(expr2_t == Inter::AND) {
        And* and = (And*)expr2;
        and->jumping(0, f);
    }
    else if(expr2_t == Inter::REL) {
        Rel* rel = (Rel*)expr2;
        rel->jumping(0, f);
    }
    else if(expr2_t == Inter::CONS) {
        Constant* cons = (Constant*)expr2;
        cons->jumping(0, f);
    }
    else if(expr2_t == Inter::ID) {
        Id* id = (Id*)expr2;
        id->jumping(0, f);
    }
    else {
        this->expr1->jumping(0, f);
    }
    if (f == 0) {
        this->emitlabel(label);
    }
}

void Or::jumping(int t, int f) {
    int label = t ? t : this->newlable();
    int expr1_t = this->expr1->getClazz();
    int expr2_t = this->expr2->getClazz();
    if(expr1_t == Inter::OR) {
        Or* or = (Or*)expr1;
        or->jumping(label, 0);
    }
    else if(expr1_t == Inter::AND) {
        And* and = (And*)expr1;
        and->jumping(label, 0);
    }
    else if(expr1_t == Inter::REL) {
        Rel* rel = (Rel*)expr1;
        rel->jumping(label, 0);
    }
    else if(expr1_t == Inter::CONS) {
        Constant* cons = (Constant*)expr1;
        cons->jumping(label, 0);
    }
    else if(expr1_t == Inter::ID) {
        Id* id = (Id*)expr1;
        id->jumping(0, f);
    }
    else if(expr1_t == Inter::ID) {
        Id* id = (Id*)expr1;
        id->jumping(0, f);
    }
    else {
        this->expr1->jumping(label, 0);
    }
    if(expr2_t == Inter::OR) {
        Or* or = (Or*)expr2;
        or->jumping(label, 0);
    }
    else if(expr2_t == Inter::AND) {
        And* and = (And*)expr2;
        and->jumping(t, f);
    }
    else if(expr2_t == Inter::REL) {
        Rel* rel = (Rel*)expr2;
        rel->jumping(t, f);
    }
    else if(expr2_t == Inter::CONS) {
        Constant* cons = (Constant*)expr2;
        cons->jumping(t, f);
    }
    else if(expr2_t == Inter::ID) {
        Id* id = (Id*)expr2;
        id->jumping(t, f);
    }
    else {
        this->expr1->jumping(t, f);
    }
    if (t == 0) {
        this->emitlabel(label);
    }
}

void Rel::jumping(int t, int f) {
    int expr1_t = this->expr1->getClazz();
    int expr2_t = this->expr2->getClazz();
    int op_t    = this->op->getClazz();
    std::string str1, str2, strop;
    if(expr1_t == Inter::CONS) {
        log_msg("constant->reduce->tostr");
        Constant* cons = (Constant*)expr1;
        str1 = cons->reduce()->toString();
    }
    else if(expr1_t == Inter::ARIT) {
        Arith* arith = (Arith*)expr1;
        str1 = arith->reduce(arith)->toString();
    }
    else {
        str1 = expr1->reduce()->toString();
    }
    if(expr2_t == Inter::CONS) {
        log_msg("constant->reduce->tostr");
        Constant* cons = (Constant*)expr2;
        str2 = cons->reduce()->toString();
    }
    else if(expr2_t == Inter::ARIT) {
        Arith* arith = (Arith*)expr2;
        str2 = arith->reduce(arith)->toString();
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