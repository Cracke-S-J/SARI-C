// Copyright (c) ssj. All rights reserved.

#pragma once

#include "symbol.h"
#include "token.h"

#include <string>
#include <map>
#include <fstream>

std::string VERSION = "1.0";

class Lexer {
private:
    int line;
    std::fstream &stream_plain;
    std::map<std::string, Word*> words;
    char peek = ' ';
    void reserve(Word* word) {
        this->words.insert(std::map<std::string, Word*>::value_type(word->getLexeme(), word));
    }
    void readch() {
        stream_plain.get(this->peek);
    }
    bool match(char ch) {
        this->readch();
        if (ch != this->peek) {
            return false;
        }
        this->peek = ' ';
        return true;
    }
    void* gen_token(char ch, Word* word) {
        if (this->match(ch)) {
            return word;
        }
        else {
            return (Token*)new Token(ch);
        }
    }

public:
    Lexer(std::fstream &in_) : stream_plain(in_), line(0) {
        this->reserve((Word*)new Word("if",    Tags::IF));
        this->reserve((Word*)new Word("else",  Tags::ELSE));
        this->reserve((Word*)new Word("while", Tags::WHILE));
        this->reserve((Word*)new Word("do",    Tags::DO));
        this->reserve((Word*)new Word("break", Tags::BREAK));
        this->reserve((Word*)new Word("true",  Tags::TRUE));
        this->reserve((Word*)new Word("false", Tags::FALSE));
        this->reserve((Word*)new Word("char",  Tags::BASIC));
        this->reserve((Word*)new Word("int",   Tags::BASIC));
        this->reserve((Word*)new Word("float", Tags::BASIC));
        this->reserve((Word*)new Word("bool",  Tags::BASIC));
    }
    ~Lexer() {
        for(auto iter : this->words) {
            delete(iter.second);
        }
    }

    int getLine() const{
        return this->line;
    }

    void* scan() {
        while(true) {
            if (this->peek == ' ' || this->peek == '\t')
                ;
            else if (this->peek == '\n')
                this->line++;
            else
                break;
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
            return this->gen_token('!', Words.eq);
        case '<':
            return this->gen_token('<', Words.eq);
        case '>':
            return this->gen_token('>', Words.eq);
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
                return (Number*)new Number(tv);
            }
            readch();
            float ftv = (float)tv;
            float dv = 10.0;
            while (isdigit(this->peek)) {
                ftv += (this->peek - '0') / dv;
                dv *= 10.0;
                this->readch();
            }
            return (Real*)new Real(ftv);
        }
        if (isalpha(this->peek)) {
            std::string lexeme = "";
            while (isalpha(this->peek) || isdigit(this->peek)) {
                lexeme += this->peek;
                readch();
            }
            auto iter = this->words.find(lexeme);
            if (iter != this->words.end()) {
                return iter->second;
            }
            Word* word = new Word(lexeme, Tags::ID);
            this->words.insert(std::map<std::string, Word*>::value_type(lexeme, word));
            return word;
        }
        Token* token = new Token(this->peek);
        this->peek = ' ';
        return token;
    }
};