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
    static std::string userRole;
    static int userID;
    
    std::string toHexString(const unsigned char* data, size_t length);
    
public:
    struct UserDetails
    {
        int id;
        std::string role;
    };

    std::string generateSalt(size_t length = 16);
    std::string hashPassword(const std::string &password, const std::string &salt);
    bool verifyPassword(const std::string &password, const std::string &salt, const std::string &hash);

    static std::map<std::string, std::string> login(std::string username, std::string password);
    static void logout();
    static bool isLoggedIn();
    static bool usingPresetPassword();
    UserDetails retrieveLoggedUserDetails();
    static bool isAdmin();
};

#endif