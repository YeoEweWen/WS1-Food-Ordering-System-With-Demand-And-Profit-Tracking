#include "ui/profile.h"
#include "ui/ui_manager.h"
#include "core/users.h"
#include "core/auth.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

void ProfileUI::main() {
    Page page = UIManager::currentPageDetails();

    UIManager::header("Profile");
    UIManager::errorMessages(page.errorMessages, false);
    UIManager::infoMessages(page.infoMessages, false);

    map<string, string> params = page.params;

    UIManager::clearErrorMessages();
    UIManager::clearInfoMessages();

    Users users;

    int id = Auth::retrieveLoggedUserDetails().id;

    Users::UserDetails details = users.userDetails(id);

    cout<<"Name           : "<<details.name<<endl;
    cout<<"Username       : "<<details.username<<endl;
    cout<<"Role           : "<<details.role<<endl;
    cout<<"Status         : "<<details.status<<endl;
    cout<<"Registered By  : "<<details.registeredByName<<endl;
    cout<<"Registered At  : "<<details.registeredAt<<endl;
    cout<<"Last Logged In : "<<details.lastLoggedIn<<endl;

    cout<<string(UIManager::getlineLength(), '-')<<endl;

    cout<<"Actions:"<<endl;
    cout<<"[U] Update Name"<<endl;
    cout<<"[C] Change Password"<<endl;
    cout<<"\n[X] Back"<<endl;

    cout<<"\nOption > ";
    string input = UIManager::checkPresetInput();

    string name, newPassword, confirmPassword, selectedOption;

    if (toUpperCase(input) == "U" || toUpperCase(input) == "C"){

        selectedOption = toUpperCase(input);
        if (toUpperCase(input) == "U"){ // Name
            cout<<"\nName : "<<endl;
            cout<<"> ";
            getline(cin, name);
        }
        else{ // Password - Include current password for validation?
            cout<<"Password Requirements:"<<endl;
            cout<<"1. Must be longer than 8 characters."<<endl;
            cout<<"2. Must have at least one uppercase character."<<endl;
            cout<<"3. Must have at least one lowercase character."<<endl;
            cout<<"4. Must have at least one digit."<<endl;
            cout<<"5. Must have at least one special character."<<endl;

            cout<<"\nNew Password : "<<endl;
            cout<<"> ";
            newPassword = UIManager::inputPassword();
            cout<<"\nConfirm Password : "<<endl;
            cout<<"> ";
            confirmPassword = UIManager::inputPassword();
        }
        
        cout<<endl<<string(UIManager::getlineLength(), '-')<<endl;
        cout<<"Actions:"<<endl;
        cout<<"[S] Submit"<<endl;
        cout<<"[C] Clear All"<<endl;
        cout<<"[X] Back"<<endl;

        cout<<"\nSelect Option > ";
        getline(cin, input);

        if (toUpperCase(input) == "S"){ // Submit
            UIManager::addPresetInput(selectedOption);
            if (toUpperCase(selectedOption) == "U"){ // Name
                if (Validation::validateName(name)){
                    if (users.updateName(name)){
                        UIManager::addInfoMessage("Your name has been successfully updated.");
                        UIManager::clearPresetInputs();
                        Auth::updateName(name);
                    }
                    else{
                        UIManager::addErrorMessage("Failed to update the name.");
                    }
                }
                else{
                    UIManager::addErrorMessage("Name must be between 3 and 300 characters.");
                }
            }
            else{ // Password
                bool isValid = true;

                if (!Validation::validatePassword(newPassword)){
                    UIManager::addErrorMessage("Password did not meet the requirements.");
                    isValid = false;
                }

                if (!Validation::validateConfirmPassword(newPassword, confirmPassword)){
                    UIManager::addErrorMessage("Passwords did not match.");
                    isValid = false;
                }

                if (isValid){
                    if (users.updatePassword(newPassword)){
                        UIManager::addInfoMessage("Your password has been successfully updated.");
                        UIManager::clearPresetInputs();
                    }
                    else{
                        UIManager::addErrorMessage("Failed to update the password.");
                    }
                }
            }
        }
        else if (toUpperCase(input) == "C"){ // Clear All
            UIManager::addPresetInput(selectedOption);
            // Re-render the UI
        }
        else if (toUpperCase(input) == "X"){ // Back
            UIManager::goTo(8);
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
            UIManager::addPresetInput(selectedOption);
        }

    }
    else if (toUpperCase(input) == "X"){ // Back
        UIManager::clearParams();
        UIManager::clearPresetInputs();
        UIManager::goTo(1);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}
