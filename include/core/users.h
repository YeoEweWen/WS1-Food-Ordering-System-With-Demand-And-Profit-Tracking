#ifndef USERS_H
#define USERS_H

#include "core/database.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>
#include "utils/functions.h"

class Users {       
private:
    std::string generateUsername(std::string name);
    bool isValidRole(std::string role);

public:
    struct UserDetails {
        int id;
        std::string name = "";
        std::string username = "";
        std::string role = "";
        std::string status = "";
        std::string lastLoggedIn = "";
        std::string registeredAt = "";
        int registeredByID = -1;
        std::string registeredByName = "";
    };

    // Admin Only
    bool registerUser(std::string name, std::string role);
    bool updateRole(int id, std::string newRole);
    bool deactivate(int id);
    bool activate(int id);
    bool resetPassword(int id);
    TableList userList(std::string search="", std::string sortColumn="", bool sortAsc=true, int page=1, int limitRowPerPage=25);
    UserDetails userDetails(int id);

    // All Users
    bool updateName(std::string newName);
    bool updatePassword(std::string newPassword);
};

#endif