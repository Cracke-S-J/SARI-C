// Copyright (c) ssj. All rights reserved.

#pragma once

#include <string>
#include <sstream>

#include "log.h"

class Tags {
public:
    static const int AND   = 256;
    static const int BASIC = 257;
    static const int BREAK = 258;
    static const int DO    = 259;
    static const int ELSE  = 260;
    static const int EQ    = 261;
    static const int FALSE = 262;
    static const int GE    = 263;
    static const int ID    = 264;
    static const int IF    = 265;
    static const int INDEX = 266;
    static const int LE    = 267;
    static const int MINUS = 268;
    static const int NE    = 269;
    static const int NUM   = 270;
    static const int OR    = 271;
    static const int REAL  = 272;
    static const int TEMP  = 273;
    static const int TRUE  = 274;
    static const int WHILE = 275;
};

class Clazz {
public:
    static const int TOKEN = 0;
    static const int NUM   = 1;
    static const int REAL  = 2;
    static const int WORD  = 3;
};

class Token {
private:
    int tag;
    int clazz;
public:
    Token(){}
    ~Token(){}
    Token(const int tag) {
        this->tag = tag;
    }
    int getTag() const{
        return this->tag;
    }
    int getClazz() const{
        return this->clazz;
    }
    void setClazz(int c) {
        this->clazz = c;
        return;
    }
    std::string toString() const{
        if(this->tag < 256) {
            std::stringstream sstr;
            sstr << (char)this->tag;
            return sstr.str();
        }
        return std::to_string(this->tag);
    }
};

class Number : public Token {
private:
    int value;
public:
    Number(){}
    ~Number(){}
    Number(int value) : Token(Tags::NUM) {
        this->value = value;
    }
    int getNumber() const{
        return this->value;
    }
    std::string toString() const{
        return std::to_string(this->value);
    }
};

class Real : public Token {
private:
    float value;
public:
    Real(){}
    ~Real(){}
    Real(float value) : Token(Tags::REAL) {
        this->value = value;
    }
    float getNumber() const{
        return this->value;
    }
    std::string toString() const{
        return std::to_string(this->value);
    }
};

class Word : public Token {
private:
    std::string lexeme;
public:
    Word(){}
    ~Word(){}
    Word(std::string lexeme, int tag) : Token(tag) {
        this->lexeme = lexeme;
    }
    std::string getLexeme() const{
        return this->lexeme;
    }
    std::string toString() const{
        return this->lexeme;
    }
};

class WORDS{
public:
    Word* and   = new Word("&&", Tags::AND);
    Word* or    = new Word("||", Tags::OR);
    Word* eq    = new Word("==", Tags::EQ);
    Word* ne    = new Word("!=", Tags::NE);
    Word* le    = new Word("<=", Tags::LE);
    Word* ge    = new Word(">=", Tags::GE);
    Word* minus = new Word("minus", Tags::MINUS);
    Word* True  = new Word("true", Tags::TRUE);
    Word* False = new Word("false", Tags::FALSE);
    Word* temp  = new Word("t", Tags::TEMP);
};
