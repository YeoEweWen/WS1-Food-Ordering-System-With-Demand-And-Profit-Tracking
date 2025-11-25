#ifndef USERS_H
#define USERS_H

#include "database.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>

class Users {     
private:
    std::string generateUsername(std::string name);
    bool isValidRole(std::string role);

public:
    // Admin Only
    std::map<std::string, std::string> registerUser(std::string name, std::string role);
    bool updateRole(int id, std::string newRole);
    bool deactivate(int id);
    bool activate(int id);
    bool resetPassword(int id);
    std::vector<std::map<std::string, std::string>> userList();

    // All Users
    bool updateName(std::string newName);
    bool updatePassword(std::string newPassword);
};

#endif