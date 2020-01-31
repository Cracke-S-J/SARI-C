// Copyright (c) ssj. All rights reserved.

#pragma once

#define TODO 0

#include <string>
#include <iterator>
#include <map>

#include "token.h"
#include "inter.h"


class Environ {
private:
    std::map<Token*, Id*> table;
    Environ* prev;
public:
    Environ() {
        this->prev = nullptr;
    }
    Environ(Environ* pre) :prev(pre) {}
    ~Environ() {}
    void insert(Token* w, Id* i) {
        // ???
        table.insert(std::map<Token*, Id*>::value_type(w, i));
    }
    std::string get(std::string w) {
        Environ* tmp = this;
        while (tmp) {
            # if TODO
            auto iter = tmp->table.find(w);
            if (iter != tmp->table.end()) {
                return iter->second;
            }
            # endif
            tmp = tmp->prev;
        }
        return nullptr;
    }
};

class Type : public Word {
protected:
    int width;
public:
    Type() : width(0) {}
    ~Type(){}
    Type(std::string str, int tag, int width) : width(width), Word(str, tag) {}
    
    int getWidth() const{
        return width;
    }

    bool isChar() {
        return !this->toString().compare("char");
    }
    bool isInt() {
        return !this->toString().compare("int");
    }
    bool isFloat() {
        return !this->toString().compare("float");
    }
    bool isBool() {
        return !this->toString().compare("bool");
    }

    static bool numeric(Type* t) {
        return t->isBool()? false : true;
    }

    static Type* max(Type* a, Type* b) {
        if (!Type::numeric(a) || !Type::numeric(b)) {
            return nullptr;
        }
        if (a->isFloat() || b->isFloat()) {
            return a->isFloat() ? a : b;
        }
        if (a->isInt() || b->isInt()) {
            return a->isInt() ? a : b;
        }
        return a;
    }
};

class TYPES {
public:
    Type* Char  = new Type("char", Tags::BASIC, 1);
    Type* Bool  = new Type("bool", Tags::BASIC, 1);
    Type* Int   = new Type("int",  Tags::BASIC, 4);
    Type* Float = new Type("float", Tags::BASIC, 8);
}Types;

class Array : public Type {
private:
    int size;
    Array* of;
public:
    Array(){}
    ~Array(){}
    Array(int size, Array* pre) : Type("[]", Tags::INDEX, size*pre->width) {
        this->size = size;
        this->of = pre;
    }
    std::string toString() const {
        std::string str("[");
        str.append(std::to_string(this->size));
        str.append("]");
        return str + of->toString();
    }
};
