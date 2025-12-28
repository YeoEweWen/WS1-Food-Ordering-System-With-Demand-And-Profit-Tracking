#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib> // for stoi, stod
#include <iostream>
#include <fstream>
#include <ctime>

// Global Stuctures
struct OperationResult {
    bool success = false;        // true if operation succeeded
    std::string message;         // descriptive message
    int errorCode = -1;          // error code = -1, mean no error.
};

struct TableList {
    std::vector<std::map<std::string, std::string>> list;
    int totalRows;
};

// ----- Logger -----
void logError(std::string message);
void logInfo(std::string message);

// ----- Universal -----
std::string timestamp();
bool isInteger(const std::string& s);
bool isFloat(const std::string& s);
int countStringLength(const std::string& text);
void printVectorValues(const std::vector<std::map<std::string, std::string>>& mapValue);
std::string toProperCase(const std::string& input);
std::string toUpperCase(const std::string& input);
std::string toLowerCase(const std::string& input);
std::vector<std::string> split(const std::string &text, char delimiter);
std::string trim(const std::string& s);

#endif
