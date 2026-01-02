#include "ui/main_menu.h"
#include "ui/ui_manager.h"
#include "core/auth.h"

using namespace std;

void MainMenuUI::main(){
    UIManager::header("Main Menu");
    UIManager::errorMessages(UIManager::currentPageDetails().errorMessages, false);

    string option;

    if (Auth::isAdmin()){
        cout<<"[1] Users Management"<<endl;
        cout<<"[2] Menu Management"<<endl;
        cout<<"[3] Order System"<<endl;
        cout<<"[4] Transaction History"<<endl;
        cout<<"[5] Analysis & Reports"<<endl;
        cout<<"[6] Store"<<endl;
        cout<<"[7] Profile"<<endl<<endl;
    }
    else{
        cout<<"[1] Order System"<<endl;
        cout<<"[2] Transaction History"<<endl;
        cout<<"[3] Profile"<<endl<<endl;
    }
    cout<<"[X] Logout"<<endl<<endl;

    cout<<"Select Option > ";
    getline(cin, option);

    UIManager::clearErrorMessages();

    if (option == "1" && Auth::isAdmin()){
        // Users Management
        UIManager::goTo(2);
    }
    else if (option == "2" && Auth::isAdmin()){
        // Menu Management
        UIManager::goTo(3);
    }
    else if ((option == "3" && Auth::isAdmin()) || (option == "1" && !Auth::isAdmin())){
        // Order System
        UIManager::goTo(4);
    }
    else if ((option == "4" && Auth::isAdmin()) || (option == "2" && !Auth::isAdmin())){
        // Transaction History
        UIManager::goTo(5);
    }
    else if (option == "5" && Auth::isAdmin()){
        // Analysis & Reports
        UIManager::goTo(6);
    }
    else if (option == "5" && Auth::isAdmin()){
        // Store
        UIManager::goTo(7);
    }
    else if ((option == "6" && Auth::isAdmin()) || (option == "3" && !Auth::isAdmin())){
        // Profile
        UIManager::goTo(8);
    }
    else if (option == "x" || option == "X"){
        Auth::logout();
        UIManager::goTo(0);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}