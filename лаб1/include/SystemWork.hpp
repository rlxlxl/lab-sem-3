#pragma once

#include "Array.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

Array<string> write_in_dbms(string path) {
    Array<string> lines = create_array_ar<string>(10);
    fstream test(path);
    if (test.is_open()) {
        cout << "File opened successfully" << endl;
    } else {
        cout << "File not found" << endl;
    }
    string line;
    while (getline(test, line)) {
        stringstream ss(line);
        string word;
        while (ss >> word) {
            add_element_end_ar<string>(lines, word);
        }
    }
    return lines;
}

Array<string> read_command(const char* command) {
    string str_command(command);
    string token;
    stringstream ss(str_command);
    Array<string> tokens = create_array_ar<string>(10);
    while (ss >> token) {
        add_element_end_ar<string>(tokens, token);
    }
    return tokens;
}