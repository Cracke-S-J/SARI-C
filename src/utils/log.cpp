// Copyright (c) ssj. All rights reserved.

#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void log_msg(string s) {
    ofstream outfile;
    outfile.open("..\\sari.log", ios::app);
    outfile << "[DEBUG]: " << s << endl;
    outfile.close();
    return;
}