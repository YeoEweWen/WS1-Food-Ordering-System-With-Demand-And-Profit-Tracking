#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <iostream>

class Validation {    
public:
    static bool validateName(std::string name);
    static bool validateMenuName(std::string name);
    static bool validateMenuCategoryName(std::string name);
    static bool validateStoreName(std::string name);
    static bool validatePassword(std::string password);
    static bool validateConfirmPassword(std::string password, std::string confirmPassword);
};

#endif