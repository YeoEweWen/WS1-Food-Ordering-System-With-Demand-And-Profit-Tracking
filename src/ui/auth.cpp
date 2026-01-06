#include "ui/auth.h"
#include "ui/ui_manager.h"
#include "core/auth.h"
#include "core/users.h"
#include "core/validation.h"

using namespace std;

void AuthUI::login() {
    // Login Page
    string username, password;

    UIManager::header("User Login");
    UIManager::errorMessages(UIManager::currentPageDetails().errorMessages, false);

    cout<<"Enter your username > ";
    getline(cin, username);

    cout<<"Enter your password > "; // Maybe can hide password in future
    password = UIManager::inputPassword();

    Auth::LoginStatus status = Auth::login(username, password);

    UIManager::clearErrorMessages();

    if (!status.success){
        UIManager::addErrorMessage(status.description);
        return;
    }

    if (status.usingPresetPassword){
        UIManager::goTo(0); // Update Password
        return;
    }

    UIManager::goTo(1); // Dashboard
}

void AuthUI::updatePassword() {
    // Update Password Page (for Preset Password only)
    string password, confirmPassword;

    UIManager::header("Security");
    UIManager::errorMessages(UIManager::currentPageDetails().errorMessages, false);

    cout<<"It looks like you're using a preset password." <<endl;
    cout<<"For your security, please update your password." <<endl<<endl;

    cout<<"Password Requirements:"<<endl;
    cout<<"1. Must be longer than 8 characters."<<endl;
    cout<<"2. Must have at least one uppercase character."<<endl;
    cout<<"3. Must have at least one lowercase character."<<endl;
    cout<<"4. Must have at least one digit."<<endl;
    cout<<"5. Must have at least one special character."<<endl<<endl;

    cout<<"Enter your new password > ";
    getline(cin, password);
    cout<<"Re-confirm your new password > ";
    getline(cin, confirmPassword);

    UIManager::clearErrorMessages();

    if (!Validation::validatePassword(password)){
        UIManager::addErrorMessage("Password did not meet the requirements.");
        return;
    }

    if (!Validation::validateConfirmPassword(password, confirmPassword)){
        UIManager::addErrorMessage("Passwords did not match.");
        return;
    }

    Users users;

    if (!users.updatePassword(password)){
        UIManager::addErrorMessage("Something went wrong. Please try again.");
        return;
    };

    UIManager::goTo(1); // Dashboard
}