#include "ui/loginUI.h"
#include "core/auth.h"

using namespace std;

string LoginUI::errorMessage = "";

void LoginUI::main() {
    // Login Page

    string username, password;

    cout<<"Welcome to Ahmad Cafe"<<endl;
    cout<<"=================================="<<endl;

    if (LoginUI::errorMessage != ""){
        // Improve the error message
        cout<<"Error: "<<LoginUI::errorMessage<<endl;
        cout<<"=================================="<<endl;
    }

    cout<<"Username: ";
    cin>>username;
    cout<<"Password: ";
    cin>>password;

    Auth::LoginStatus status = Auth::login(username, password);

    if (!status.success){
        LoginUI::errorMessage = status.description;
        return;
    }

    LoginUI::errorMessage = "";
    
    if (status.usingPresetPassword){
        updatePassword();
    }
}

void LoginUI::updatePassword() {
    // Update Password Page (for Preset Password only)
    cin;
}