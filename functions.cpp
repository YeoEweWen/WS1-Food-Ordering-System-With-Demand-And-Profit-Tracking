#include "functions.h"
#include <string>
#include <map>
#include <vector>
#include <cstdlib> // for stoi, stod
#include <iostream>

using namespace std;

bool isInteger(const string& s) {
    if (s.empty()) return false;
    try {
        size_t pos;
        stoi(s, &pos);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}

bool isFloat(const string& s) {
    if (s.empty()) return false;
    try {
        size_t pos;
        stod(s, &pos);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}

void printVectorValues(const vector<map<string, string>>& mapValue){
    // Display the retrieved rows
    for (const auto& row : mapValue) {
        cout << "---- ROW ----" << endl;
        for (const auto& kv : row) {
            cout << kv.first << " = " << kv.second << endl;
        }
        cout << endl;
    }
}
