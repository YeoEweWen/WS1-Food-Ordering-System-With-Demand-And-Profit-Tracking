#ifndef USERS_H
#define USERS_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

class Users {    
public:
    // Admin Only
    bool registerUser(std::string name, std::string username, std::string role, std::string password);
    bool updateUsername(int id, std::string newUsername);
    bool updateRole(int id, std::string newRole);
    bool deactivate(int id);
    bool activate(int id);
    bool resetPassword(int id, std::string newPassword);
    std::vector<std::map<std::string, std::string>> userList();

    // All Users
};

#endif