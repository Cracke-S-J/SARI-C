// Copyright (c) ssj. All rights reserved.

#include "lexer.h"

#include <iostream>

class Node {
private:
    int labels;
    int lexline;
public:
    Node() : labels(0), lexline(Lexer::line) {}
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

class Stmt : public Node {
private:
    int after;
public:
    Stmt() : after(0) {}
    void gen(int before, int after) {}
    int getAfter() const{
        return after;
    }
    void setAfter(int a) {
        this->after = a;
    }
};

class STMTS {
public:
    Stmt* Null = new Stmt();
    Stmt* Enclosing = Null;
};
STMTS Stmts;

class Expr : public Node {
protected:
    Word* op;
    Type* type;
public:
    Expr(){}
    Expr(Word* tok, Type* type) : op(tok), type(type) {}
    Type* getType() const{
        return this->type;
    }
    Token* getWord() const{
        return this->op;
    }
    Expr* gen() {
        return this;
    }
    Expr* reduce() {
        return this;
    }
    std::string toString() const{
        return this->op->toString();
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

class Temp : public Expr {
private:
    int number;
public:
    static int count;
    Temp(){}
    Temp(Type* type) : Expr(Words.temp, type) {
        count++;
        this->number = count;
    }
    std::string toString() const{
        return "t" + std::to_string(number);
    }
};

class Logical : public Expr {
protected:
    Expr* expr1;
    Expr* expr2;
    Type* type;
    Type* check(Type* a, Type* b) {
        if (!a->getLexeme().compare("bool") &&
            !b->getLexeme().compare("bool")) {
            return new Type("bool", Tags::BASIC, 1);
        }
        else{
            return nullptr;
        }
    }
public:
    Logical(){}
    Logical(Word* tok, Expr* x1, Expr* x2) : Expr(tok, nullptr) {
        this->expr1 = x1;
        this->expr2 = x2;
        this->type  = this->check(x1->getType(), x2->getType());
        if (this->type == nullptr) {
            std::cout << "type error" << std::endl;
            exit(0);
        }
    }
    std::string toString() const{
        return this->expr1->toString() + " " + this->op->toString() + " " + this->expr2->toString();
    }
    Temp* gen() {
        int f = this->newlable();
        int a = this->newlable();
        Temp* tmp = new Temp(this->type);
        this->jumping(0, f);
        this->jumping(0, f);
        this->emit(tmp->toString() + " = true");
        this->emit("goto L" + a);
        this->emitlabel(f);
        this->emit(tmp->toString() + " = false");
        this->emitlabel(a);
        return tmp;
    }
};

class Op : public Expr {
public:
    Op(){}
    ~Op(){}
    Op(Word* op, Type* type) : Expr(op, type) {}
    Temp* reduce() {
        Expr* x = this->gen();
        Temp* t = new Temp(this->type);
        this->emit(t->toString() + " = " + x->toString());
        return t;
    }
};

class Id : public Expr {
private:
    int offset;
public:
    Id(){}
    ~Id(){}
    Id(Word* tok, Type* type, int b) : offset(b), Expr(tok, type) {}
};

class Unary : public Expr {
private:
    Expr* expr;
    Type* type;
public:
    Unary(){}
    ~Unary(){}
    Unary(Word* tok, Expr* x) : Expr(tok, nullptr) {
        this->expr = x;
        this->type = Type::max(Types.Int, this->expr->getType());
        if (this->type == nullptr) {
            std::cout << "type error" << std::endl;
            exit(0);
        }
    }
    Unary* gen() {
        return new Unary(this->op, this->expr->reduce());
    }
    std::string toString() {
        return this->op->toString() + " " + this->expr->toString();
    }
};

class Constant : public Expr {
public:
    Constant(){}
    ~Constant(){}
    Constant(Word* tok, Type* type) : Expr(tok, type) {}
    void jumping(int t, int f) {
        if (!t && !this->getWord()->toString().compare("true")) {
            this->emit("goto L" + std::to_string(t));
        }
        else if (!f && !this->getWord()->toString().compare("false")) {
            this->emit("goto L" + std::to_string(f));
        }
    }
};

class CONSTANTS {
public:
    Constant* True  = new Constant(Words.True,  Types.Bool);
    Constant* False = new Constant(Words.False, Types.Bool);
}Constants;
