#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <map>
#include <vector>

// Global Stuctures
struct OperationResult {
    bool success = false;        // true if operation succeeded
    std::string message;         // descriptive message
    int errorCode = -1;           // error code = -1, mean no error.
};

bool isInteger(const std::string& s);
bool isFloat(const std::string& s);
void printVectorValues(const std::vector<std::map<std::string, std::string>>& mapValue);

#endif
