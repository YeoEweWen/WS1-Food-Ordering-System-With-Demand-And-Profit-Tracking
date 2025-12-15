#ifndef LOGINUI_H
#define LOGINUI_H

#include <string>
#include <iostream>

class LoginUI {
private:
    static std::string errorMessage;

    static void updatePassword();

public:
    static void main();
};

#endif
