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
    Type* type;
public:
    Arith(){}
    ~Arith(){}
    Arith(Word* tok, Expr* x1, Expr* x2) : Op(tok, nullptr) {
        this->expr1 = x1;
        this->expr2 = x2;
        this->type = Type::max(this->expr1->getType(), this->expr2->getType());
        if (this->type == nullptr) {
            std::cout << "type error" << std::endl;
            exit(0);
        }
    }
    Arith* gen() {
        return new Arith(this->op, this->expr1->reduce(), this->expr2->reduce());
    }
    std::string toString() {
        return this->expr1->toString() + " " + this->op->toString() + " " + this->expr2->toString();
    }
};
