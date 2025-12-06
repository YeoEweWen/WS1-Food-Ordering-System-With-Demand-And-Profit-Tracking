#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <map>
#include <iostream>

#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <openssl/rand.h>

class Auth {
private:
    // Variables
    static int userID;
    static std::string userName;
    static std::string userRole;
    
    static std::string toHexString(const unsigned char* data, size_t length);
    
public:
    struct UserDetails
    {
        int id;
        std::string name;
        std::string role;
    };

    struct LoginStatus
    {
        bool success;
        int errorCode;
        std::string description;
        bool usingPresetPassword;
    };

    static std::string generateSalt(size_t length = 16);
    static std::string hashPassword(const std::string &password, const std::string &salt);
    static bool verifyPassword(const std::string &password, const std::string &salt, const std::string &hash);

    static LoginStatus login(std::string username, std::string password);
    static void logout();
    static bool isLoggedIn();
    static UserDetails retrieveLoggedUserDetails();
    static bool isAdmin();
};

#endif