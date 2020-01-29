// Copyright (c) ssj. All rights reserved.


#include<string>

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

class Token {
private:
    int tag;
public:
    Token(){}
    Token(const int tag) {
        this->tag = tag;
    }
    std::string toString() const{
        return std::to_string(this->tag);
    }
};

class Number : public Token {
private:
    int value;
public:
    Number(){}
    Number(int value) : Token(Tags::NUM) {
        this->value = value;
    }
    std::string toString() const{
        return std::to_string(this->value);
    }
};

class Real : public Token {
private:
    int value;
public:
    Real(){}
    Real(int value) : Token(Tags::REAL) {
        this->value = value;
    }
    std::string toString() const{
        return std::to_string(this->value);
    }
};

class Word : public Token {
public:
    std::string lexeme;
    
    Word(){}
    Word(std::string lexeme, int tag) : Token(tag) {
        this->lexeme = lexeme;
    }
    std::string toString() const{
        return this->lexeme;
    }
};

class Words{
public:
    Word aNd = Word("&&", Tags::AND);
    Word oR = Word("||", Tags::OR);
    Word eq = Word("==", Tags::EQ);
    Word ne = Word("!=", Tags::NE);
    Word le = Word("<=", Tags::LE);
    Word ge = Word(">=", Tags::GE);
    Word minus = Word("minus", Tags::MINUS);
    Word tRuE = Word("true", Tags::TRUE);
    Word fAlSe = Word("false", Tags::FALSE);
    Word temp = Word("t", Tags::TEMP);
};
