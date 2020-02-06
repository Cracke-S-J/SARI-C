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
    Arith* gen() {
        if(this->expr1->getClazz() == Inter::ARIT &&
           this->expr2->getClazz() == Inter::ARIT) {
            Arith* arith = (Arith*)this->expr1;
            Arith* arith2 = (Arith*)this->expr2;
            return new Arith(this->op, arith->reduce(),
                             arith2->reduce());
        }
        else if(this->expr1->getClazz() == Inter::ARIT) {
            Arith* arith = (Arith*)this->expr1;
            log_msg("expr1=arith, expr2=num");
            return new Arith(this->op, arith->reduce(),
                             this->expr2->reduce());
        }
        else if(this->expr2->getClazz() == Inter::ARIT) {
            Arith* arith = (Arith*)this->expr2;
            return new Arith(this->op, this->expr1->reduce(),
                             arith->reduce());
        }
        else {
            return new Arith(this->op, this->expr1->reduce(),
                             this->expr2->reduce());
        }
    }
    std::string toString() {
        if(this->expr1->getClazz() == Inter::TMEP) {
            Temp* t  = (Temp*)this->expr1;
            Temp* t2 = (Temp*)this->expr2;
            return t->toString() + " " + \
                    this->op->toString() + " " + \
                    t2->toString();
        }
        else if(this->expr1->getClazz() == Inter::TMEP) {
            Temp* t = (Temp*)this->expr1;
            return t->toString() + " " + \
                    this->op->toString() + " " + \
                    this->expr2->toString();
        }
        else if(this->expr2->getClazz() == Inter::TMEP) {
            Temp* t = (Temp*)this->expr1;
            return this->expr1->toString() + " " + \
                    this->op->toString() + " " + \
                    t->toString();
        }
        else {
            return this->expr1->toString() + " " + \
                    this->op->toString() + " " + \
                    this->expr2->toString();
        }
    }
};
