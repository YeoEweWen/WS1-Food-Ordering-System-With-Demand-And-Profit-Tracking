#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <map>
#include <iostream>

class Auth {
private:
    // Variables
    static std::string userRole;
    static int userID;

    struct UserDetails
    {
        int id;
        std::string role;
    };
    
    
public:
    static std::map<std::string, std::string> login(std::string username, std::string password);
    static void logout();
    static bool isLoggedIn();
    UserDetails retrieveLoggedUserDetails();
    static bool isAdmin();
    static bool validatePasswordRules(std::string password);
};

#endif