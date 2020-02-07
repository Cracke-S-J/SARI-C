// Copyright (c) ssj. All rights reserved.

#pragma once

#include "inter.h"

class Access : public Op {
private:
    Expr* index;
    Array* array;
public:
    Access(){}
    ~Access(){}
    Access(Array* array, Expr* idx, Type* type) : \
        Op(new Word("[]", Tags::INDEX), type) ,\
        index(idx), array(array) {}
    Access* gen() {
        return new Access(this->array, this->index->reduce(), this->type);
    }
    void jumping(int t, int f) {
        this->emitjumps(this->reduce()->toString(), t, f);
    }
    std::string toString() {
        return this->array->toString() + " [ " + this->index->toString() + " ] ";
    }
    Array* getArray() const{
        return this->array;
    }
    Expr* getIndex() const{
        return this->index;
    }
};

class Arith : public Op {
private:
    Expr* expr1;
    Expr* expr2;
public:
    Arith(){}
    ~Arith(){}
    Arith(Token* tok, Expr* x1, Expr* x2) : Op(tok, nullptr) {
        this->expr1 = x1;
        this->expr2 = x2;
        this->type = Type::max(this->expr1->getType(),
                               this->expr2->getType());
        if (this->type == nullptr) {
            std::cout << "type error" << std::endl;
            exit(0);
        }
    }
    Temp* reduce(Arith* self) {
        log_msg("arith->reduce()");
        Arith* x = self->gen();
        Temp* t = new Temp(this->type);
        t->setClazz(Inter::TMEP);
        this->emit(t->toString() + " = " + x->toString());
        return t;
    }
    Arith* gen() {
        log_msg("arith->gen()");
        if(this->expr1->getClazz() == Inter::ARIT &&
           this->expr2->getClazz() == Inter::ARIT) {
            log_msg("expr1=arith, expr2=arith");
            Arith* arith = (Arith*)this->expr1;
            Arith* arith2 = (Arith*)this->expr2;
            Arith* ret = new Arith(this->op,
                                    arith->reduce(arith),
                                    arith2->reduce(arith2));
            ret->setClazz(Inter::ARIT);
            return ret;
        }
        else if(this->expr1->getClazz() == Inter::ARIT) {
            Arith* arith = (Arith*)this->expr1;
            log_msg("expr1=arith, expr2=num");
            Arith* ret = new Arith(this->op,
                                    arith->reduce(arith),
                                    this->expr2->reduce());
            ret->setClazz(Inter::ARIT);
            return ret;
        }
        else if(this->expr2->getClazz() == Inter::ARIT) {
            Arith* arith = (Arith*)this->expr2;
            Arith* ret = new Arith(this->op,
                                    this->expr1->reduce(),
                                    arith->reduce(arith));
            ret->setClazz(Inter::ARIT);
            return ret;
        }
        else {
            log_msg("expr1=num, expr2=num");
            Arith* ret = new Arith(this->op,
                                    this->expr1->reduce(),
                                    this->expr2->reduce());
            ret->setClazz(Inter::ARIT);
            return ret;
        }
    }
    std::string toString() {
        if(this->expr1->getClazz() == Inter::TMEP &&
           this->expr2->getClazz() == Inter::TMEP) {
            log_msg("expr1 temp, expr2 temp");
            Temp* t  = (Temp*)this->expr1;
            Temp* t2 = (Temp*)this->expr2;
            return t->toString() + " " + \
                    this->op->toString() + " " + \
                    t2->toString();
        }
        else if(this->expr1->getClazz() == Inter::TMEP) {
            log_msg("expr1 tmp, expr2 expr");
            Temp* t = (Temp*)this->expr1;
            return t->toString() + " " + \
                    this->op->toString() + " " + \
                    this->expr2->toString();
        }
        else if(this->expr2->getClazz() == Inter::TMEP) {
            log_msg("expr1 expr, expr2 tmp");
            Temp* t = (Temp*)this->expr2;
            return this->expr1->toString() + " " + \
                    this->op->toString() + " " + \
                    t->toString();
        }
        else {
            log_msg("expr expr");
            return this->expr1->toString() + " " + \
                    this->op->toString() + " " + \
                    this->expr2->toString();
        }
    }
};
