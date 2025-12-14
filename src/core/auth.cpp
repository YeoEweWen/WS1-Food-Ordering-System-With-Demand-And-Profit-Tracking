#include "core/auth.h"
#include "core/database.h"
#include "utils/functions.h"

using namespace std;

int Auth::userID = 14; // Default value, -1 = Guest
string Auth::userName = "Ali bin Abu"; // Default Value
string Auth::userRole = "Admin"; // Default Value

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

Auth::LoginStatus Auth::login(string username, string password){
    Database db;
    Auth::LoginStatus loginStatus;

    // Default values
    loginStatus.success = false;
    loginStatus.errorCode = -1;
    loginStatus.description = "No Description.";
    loginStatus.usingPresetPassword = false;

    int id;
    string query, name, role, hashedPassword, salt, status;
    vector<string> passwordAndSalt;
    map<string, string> params;

    query = "SELECT id, name, role, password, status FROM user WHERE username = :username LIMIT 1;";
    params = {
        {"username", username}
    };

    vector<map<string, string>> result = db.fetchData(query, params);

    if (result.empty()){
        loginStatus.errorCode = 0; // User not found.
        loginStatus.description = "User not found/registered.";
        return loginStatus;
    }

    id = stoi(result[0].at("id"));
    name = result[0].at("name");
    role = result[0].at("role");
    passwordAndSalt = split(result[0].at("password"), ':');
    hashedPassword = passwordAndSalt[0];
    salt = passwordAndSalt[1];
    status = result[0].at("status");

    if (!verifyPassword(password, salt, hashedPassword)){
        loginStatus.errorCode = 1; // Wrong password.
        loginStatus.description = "Wrong password.";
        return loginStatus;
    }

    if (status == "Inactive"){
        loginStatus.errorCode = 2; // User not active.
        loginStatus.description = "User not active.";
        return loginStatus;
    }

    // Add user details
    userID = id;
    userName = name;
    userRole = role;

    // Update last logged in
    query = "UPDATE user SET last_logged_in = NOW() WHERE id = :id AND status = 'Active';";
    params = {
        {"id", to_string(id)}
    };

    db.runQuery(query, params);

    loginStatus.success = true;
    loginStatus.description = "Login successfully!";
    loginStatus.usingPresetPassword = verifyPassword(username, salt, hashedPassword);

    return loginStatus;
}

void Auth::logout(){
    userID = -1;
    userName = "Guest";
    userRole = "Guest";
}

bool Auth::isLoggedIn(){
    return (userRole != "Guest" && userID != -1);
}

Auth::UserDetails Auth::retrieveLoggedUserDetails(){
    UserDetails userDetails;

    if (!isLoggedIn()){
        userDetails.id = -1;
        userDetails.name = "Guest";
        userDetails.role = "Guest";
    }
    userDetails.id = userID;
    userDetails.name = userName;
    userDetails.role = userRole;

    return userDetails;
}

bool Auth::isAdmin() {
    if (!isLoggedIn()){
        return false;
    }
    return (userRole == "Admin");
}
