// Copyright (c) ssj. All rights reserved.

#include "lexer.h"
#include "inter.h"

std::string VERSION;
int Lexer::line = 0;

void* Lexer::scan() {
    while(true) {
        if (this->peek == ' ' || this->peek == '\t')
            ;
        else if (this->peek == '\n') {
            line++;
        }
        else if(this->peek == -1) {
            break;
        }
        else {
            break;
        }
        this->readch();
    }
    switch (this->peek) {
    case '&':
        return this->gen_token('&', Words.and);
    case '|':
        return this->gen_token('|', Words.or);
    case '=':
        return this->gen_token('=', Words.eq);
    case '!':
        return this->gen_token('=', Words.ne);
    case '<':
        return this->gen_token('=', Words.le);
    case '>':
        return this->gen_token('=', Words.ge);
    default:
        break;
    }
    if (isdigit(this->peek)) {
        int tv = 0;
        while (isdigit(this->peek)) {
            tv = tv * 10 + this->peek - '0';
            this->readch();
        }
        if (this->peek != '.') {
            Number* ret = new Number(tv);
            ret->setClazz(Clazz::NUM);
            return ret;
        }
        readch();
        float ftv = (float)tv;
        float dv = 10.0;
        while (isdigit(this->peek)) {
            ftv += (this->peek - '0') / dv;
            dv *= 10.0;
            this->readch();
        }
        Real* ret = new Real(ftv);
        ret->setClazz(Clazz::REAL);
        return ret;
    }
    if (isalpha(this->peek)) {
        std::string lexeme = "";
        while (isalpha(this->peek) || isdigit(this->peek)) {
            lexeme += this->peek;
            readch();
        }
        auto iter = this->words.find(lexeme);
        if (iter != this->words.end()) {
            iter->second->setClazz(Clazz::WORD);
            return iter->second;
        }
        Word* word = new Word(lexeme, Tags::ID);
        word->setClazz(Clazz::WORD);
        this->words.insert(std::map<std::string, Word*>::value_type(lexeme, word));
        return word;
    }
    Token* token = new Token(this->peek);
    this->peek = ' ';
    token->setClazz(Clazz::TOKEN);
    return token;
}
