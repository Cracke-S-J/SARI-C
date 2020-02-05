// Copyright (c) ssj. All rights reserved.

#include "inter.h"
#include "symbol.h"
#include "lexer.h"
#include "token.h"

WORDS Words;
TYPES Types;
STMTS Stmts;
CONSTANTS Constants;

int Temp::count = 0;

Unary::Unary(Token* tok, Expr* x) : Expr(tok, nullptr) {
    this->expr = x;
    this->type = Type::max(Types.Int, this->expr->getType());
    if (this->type == nullptr) {
        std::cout << "type error" << std::endl;
        exit(0);
    }
}

CONSTANTS::CONSTANTS() {
    True  = new Constant(Words.True,  Types.Bool);
    False = new Constant(Words.False, Types.Bool);
}

Temp::Temp(Type* _type) : Expr(Words.temp, _type) {
    Temp::count++;
    this->number = count;
}
