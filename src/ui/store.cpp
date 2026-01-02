#include "core/store.h"
#include "ui/store.h"
#include "ui/ui_manager.h"
#include "core/auth.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

void StoreUI::main() {
    Page page = UIManager::currentPageDetails();

    UIManager::header("Store");
    UIManager::errorMessages(page.errorMessages, false);
    UIManager::infoMessages(page.infoMessages, false);

    map<string, string> params = page.params;

    UIManager::clearErrorMessages();
    UIManager::clearInfoMessages();

    Store::StoreDetails details = Store::details();

    cout<<"Store Name       : "<<details.name<<endl;
    cout<<"Register By      : "<<details.createdBy<<endl;
    cout<<"Register At      : "<<details.createdAt<<endl;
    cout<<"Last Update By   : "<<details.updatedBy<<endl;
    cout<<"Last Update At   : "<<details.updatedAt<<endl;

    cout<<string(UIManager::getlineLength(), '-')<<endl;

    cout<<"Actions:"<<endl;
    cout<<"[U] Update Store Name"<<endl;
    cout<<"\n[X] Back"<<endl;

    cout<<"\nOption > ";
    string input = UIManager::checkPresetInput();

    string name;

    if (toUpperCase(input) == "U"){
        cout<<"\nStore Name : "<<endl;
        cout<<"> ";
        getline(cin, name);
        
        cout<<endl<<string(UIManager::getlineLength(), '-')<<endl;
        cout<<"Actions:"<<endl;
        cout<<"[S] Submit"<<endl;
        cout<<"[C] Clear All"<<endl;
        cout<<"[X] Back"<<endl;

        cout<<"\nSelect Option > ";
        getline(cin, input);

        if (toUpperCase(input) == "S"){ // Submit
            if (Validation::validateStoreName(name)){
                if (Store::update(name)){
                    UIManager::addInfoMessage("The store name has been successfully updated.");
                    UIManager::clearPresetInputs();
                    UIManager::refreshStoreName();
                }
                else{
                    UIManager::addErrorMessage("Failed to update the store name.");
                }
            }
            else{
                UIManager::addErrorMessage("Name must be between 10 and 100 characters.");
            }
        }
        else if (toUpperCase(input) == "C"){ // Clear All
            UIManager::addPresetInput("U");
            // Re-render the UI
        }
        else if (toUpperCase(input) == "X"){ // Back
            UIManager::goTo(7);
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
            UIManager::addPresetInput("U");
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
