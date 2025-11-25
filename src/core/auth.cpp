#include "auth.h"
#include "database.h"
#include "functions.h"

using namespace std;

string Auth::userRole = "Admin"; // Default Value
int Auth::userID = 14; // Default value, -1 = Guest

string Auth::toHexString(const unsigned char* data, size_t length) {
    stringstream ss;
    for (size_t i = 0; i < length; ++i)
        ss << hex << setw(2) << setfill('0') << (int)data[i];
    return ss.str();
}

// Generate a random salt of given length
string Auth::generateSalt(size_t length) {
    unsigned char salt[16];
    if (!RAND_bytes(salt, length)) {
        throw runtime_error("Failed to generate salt");
    }
    return toHexString(salt, length);
}

// Hash password + salt
string Auth::hashPassword(const string &password, const string &salt) {
    string input = password + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);
    return toHexString(hash, SHA256_DIGEST_LENGTH);
}

// Verify password
bool Auth::verifyPassword(const string &password, const string &salt, const string &hash) {
    return hashPassword(password, salt) == hash;
}

map<string, string> Auth::login(string username, string password){
    return {};
}

void Auth::logout(){
    userRole = "Guest";
    userID = -1;
}

bool Auth::isLoggedIn(){
    return (userRole != "Guest" && userID != -1);
}

bool usingPresetPassword(){
    return false;
}

Auth::UserDetails Auth::retrieveLoggedUserDetails(){
    UserDetails userDetails;

    if (!isLoggedIn()){
        userDetails.id = -1;
        userDetails.role = "Guest";
    }
    userDetails.id = userID;
    userDetails.role = userRole;

    return userDetails;
}

bool Auth::isAdmin() {
    if (!isLoggedIn()){
        return false;
    }
    return (userRole == "Admin");
}
