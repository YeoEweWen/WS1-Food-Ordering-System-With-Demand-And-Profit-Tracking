#include "auth.h"
#include "database.h"
#include "functions.h"

using namespace std;

string Auth::userRole = "Guest"; // Default Value
int Auth::userID = -1; // Default value, -1 = Guest

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

bool Auth::validatePasswordRules(string password){
    /*
    1. Length > 8 characters.
    2. Must have at least one uppercase character.
    3. Must have at least one lowercase character.
    4. Must have at least one digit.
    5. Must have at least one special character.
    */

    if (password.length() < 8) return false;

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        else if (islower(c)) hasLower = true;
        else if (isdigit(c)) hasDigit = true;
        else hasSpecial = true;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}
