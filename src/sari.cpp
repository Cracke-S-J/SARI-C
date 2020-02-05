// Copyright (c) ssj. All rights reserved.

#include "lexer.h"
#include "parser.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "usage" << endl;
        return 0;
    }
    fstream fd;
    fd.open(argv[1], ios::in);
    Lexer* lexer  = new Lexer(fd);
    Parser parser = Parser(lexer);
    parser.program();
    return 0;
}