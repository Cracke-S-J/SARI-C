// Copyright (c) ssj. All rights reserved.


#include <string>
#include <iterator>
#include <map>
#include "token.h"


class Environ {
private:
    std::map<std::string, std::string> table;
    Environ* prev;
public:
    Environ() {
        this->prev = nullptr;
    }
    void insert(std::string w, std::string i) {
        // ???
        table.insert(std::map<std::string, std::string>::value_type(w, i));
    }
    std::string get(std::string w) {
        Environ* tmp = this;
        while (tmp) {
            auto iter = tmp->table.find(w);
            if (iter != tmp->table.end()) {
                return iter->second;
            }
            tmp = tmp->prev;
        }
        return nullptr;
    }
};

class Type : public Word {
protected:
    int width;
public:
    Type() {
        width = 0;
    }
    Type(std::string str, int tag, int width) : Word(str, tag) {
        this->width = width;
    }
    bool operator==(const Type& a) {
        return true;
        return this->width == a.width && !this->toString().compare(a.toString());
    }
    static bool numeric(Type &t) {
        if (!t.toString().compare("char") || 
            !t.toString().compare("int")  || 
            !t.toString().compare("floot")) {
            return true;
        }
        else {
            return false;
        }
    }

    static Type max(Type &a, Type &b) {
        if (!Type::numeric(a) || !Type::numeric(b)) {
            return Type();
        }
        if (!a.toString().compare("floot") || 
            !b.toString().compare("floot")) {
            return a.toString().compare("floot")?b:a;
        }
        if (!a.toString().compare("int") || 
            !b.toString().compare("int")) {
            return a.toString().compare("int")?b:a;
        }
        return a;
    }
};

class Array : public Type {
private:
    int size;
    Array* of;
public:
    Array(){}
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
