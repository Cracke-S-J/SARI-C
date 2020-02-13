// Copyright (c) ssj. All rights reserved.

#pragma once

#include "token.h"
#include "log.h"

#include <string>
#include <map>
#include <fstream>

class Lexer {
private:
    std::fstream &stream_plain;
    std::map<std::string, Word*> words;
    char peek = ' ';
    int is_eof;
    void reserve(Word* word) {
        this->words.insert(std::map<std::string, Word*>::value_type(word->getLexeme(), word));
    }
    void readch() {
        stream_plain.get(this->peek);
        if(stream_plain.eof()) {
            log_msg("EOF");
            this->peek = -1;
        }
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
            word->setClazz(Clazz::WORD);
            return word;
        }
        else {
            Token* tok = new Token(ch);
            tok->setClazz(Clazz::TOKEN);
            return tok;
        }
    }

public:
    static int line;
    Lexer(std::fstream &in_) : stream_plain(in_), is_eof(0) {
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

    void* scan();
};
