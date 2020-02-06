// Copyright (c) ssj. All rights reserved.

#include "inter.h"
#include "symbol.h"
#include "lexer.h"
#include "token.h"

WORDS Words;
TYPES Types;
STMTS Stmts;
CONSTANTS Constants(new Constant(Words.True, Types.Bool),
                    new Constant(Words.False, Types.Bool));

int Temp::count  = 0;
int Node::labels = 0;
int Node::lexline = Lexer::line;

int Node::newlable() {
    return Node::labels++;
}

Unary::Unary(Token* tok, Expr* x) : Expr(tok, nullptr) {
    this->expr = x;
    this->type = Type::max(Types.Int, this->expr->getType());
    if (this->type == nullptr) {
        std::cout << "type error" << std::endl;
        exit(0);
    }
}

Temp::Temp(Type* _type) : Expr(Words.temp, _type) {
    Temp::count++;
    this->number = count;
}
