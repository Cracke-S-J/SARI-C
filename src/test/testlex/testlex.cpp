// Copyright (c) ssj. All rights reserved.


#include <iostream>
#include <fstream>

#include "token.h"
#include "lexer.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "usage" << endl;
        return 0;
    }
    fstream fd;
    fd.open(argv[1], ios::in);
    Lexer lexer = Lexer(fd);
    for (Token* tok = (Token*)lexer.scan();
                tok->getTag() != 125;
                tok = (Token*)lexer.scan()) {
        int tag = tok->getTag();
        if (tag < 256) {
            cout << "unhandle char: " << (char)tag << endl;
        }
        else {
            Number* num = (Number*)tok;
            Real* real  = (Real*)tok;
            Word* word  = (Word*)tok;
            switch (tag) {
            case Tags::NUM:
                cout << "number: " + num->toString() << endl;
                break;
            case Tags::REAL:
                cout << "realnum: " + real->toString() << endl;
                break;
            case Tags::ID:
                cout << "id: " + word->toString() << endl;
                break;
            case Tags::BASIC:
                cout << "type: " + word->toString() << endl;
                break;
            default:
                cout << "logic: " + word->toString() << endl;
                break;
            }
        }
    }
    return 0;
}