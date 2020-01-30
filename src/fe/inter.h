// Copyright (c) ssj. All rights reserved.

#include "lexer.h"

#include <iostream>

class Node {
private:
    int labels;
    int lexline;
public:
    Node() : labels(0), lexline(Lexer::getLine()) {}
    int newlable() {
        return this->labels++;
    }
    void emitlabel(int idx) {
        std::cout << "L" << std::to_string(idx) << ":";
    }
    void emit(std::string str) {
        std::cout << "\t" << str << std::endl;
    }
    void error(std::string str) {
        std::cout << "near line " << this->lexline << str << std::endl;
    }
};

class Expr : public Node {
protected:
    Token op;
    Type type;
public:
    Expr(){}
    Expr(Token tok, Type type) : op(tok), type(type) {}
    Type getType() const{
        return this->type;
    }
    Expr* gen() {
        return this;
    }
    Expr* reduce() {
        return this;
    }
    std::string toString() const{
        return this->op.toString();
    }
    void emitjumps(std::string str, int t, int f) {
        if (t != 0 && f != 0) {
            this->emit("if " + str + " goto L" + std::to_string(t));
            emit("goto L" + std::to_string(f));
        }
        else if (t != 0) {
            this->emit("if " + str + " goto L" + std::to_string(t));
        }
        else if (f != 0) {
            this->emit("iffalse" + str + " goto L" + std::to_string(t));
        }
    }
    void jumping(int t, int f) {
        this->emitjumps(this->toString(), t, f);
    }
};

class Logical : public Expr {
private:
    Expr expr1;
    Expr expr2;
    Type* type;
    Type* check(Type a, Type b) {
        if (a == Type("bool", Tags::BASIC, 1) &&
            b == Type("bool", Tags::BASIC, 1)) {
                return new Type("bool", Tags::BASIC, 1);
            }
        else{
            return nullptr;
        }
    }
public:
    Logical(){}
    Logical(Token tok, Expr &x1, Expr &x2) : Expr(tok, nullptr) {
        this->expr1 = x1;
        this->expr2 = x2;
        this->type  = this->check(x1.getType(), x2.getType());
        if (this->type == nullptr) {
            std::cout << "type error" << std::endl;
            exit(0);
        }
    }
    std::string toString() const{
        return this->expr1.toString() + " " + this->op.toString() + " " + this->expr2.toString();
    }
    Temp* get() {
        
    }
};
