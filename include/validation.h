#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <iostream>

class Validation {    
public:
    bool validateName(std::string name);
    bool validatePassword(std::string password);
    bool validateConfirmPassword(std::string password, std::string confirmPassword);
};

#endif