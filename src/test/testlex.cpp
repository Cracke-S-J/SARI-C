// Copyright (c) ssj. All rights reserved.

#include <iostream>
#include <fstream>

#include "lexer.h"

using namespace std;

int main() {
    fstream fd;
    fd.open("example.txt", ios::in);
    Lexer lexer = Lexer(fd);
    
}