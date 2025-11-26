#include "validation.h"
#include "functions.h"

using namespace std;

bool Validation::validateName(string name){
    /*
    Min: 3 Characters
    Max: 300 Characters
    */
    return (name.length() >= 3 && name.length() <= 300);
}

bool Validation::validatePassword(string password){
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

bool Validation::validateConfirmPassword(string password, string confirmPassword){
    return (password == confirmPassword);
}
