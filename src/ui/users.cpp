#include "ui/users.h"
#include "ui/ui_manager.h"
#include "core/users.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

void UsersUI::main() {
    Page page = UIManager::currentPageDetails();

    UIManager::header("Users Management");
    UIManager::errorMessages(page.errorMessages, false);
    UIManager::infoMessages(page.infoMessages, false);

    map<string, string> params = page.params;

    UIManager::clearErrorMessages();
    UIManager::clearInfoMessages();

    switch (page.subID){
        case 1:
            if (!params.empty() && params.count("id") > 0){
                details(stoi(params.at("id")));
            }
            else{
                UIManager::addErrorMessage("Invalid User's ID!");
                UIManager::goTo(2);
            }
            break;

        case 2:
            registerUser();
            break;

        case 0:
        default:
            string search = "";
            Sort sort = {-1, true};
            int pageNum = 1;

            if (!params.empty()){
                if (params.count("search")){
                    search = params.at("search");
                }
                if (params.count("sort_column_index") > 0){
                    int columnIndex = stoi(params.at("sort_column_index"));
                    bool sortAsc = true;
                    if (params.count("sort_asc") > 0){
                        sortAsc = toLowerCase(params.at("sort_asc")) != "false";        
                    }

                    sort = {columnIndex, sortAsc};
                }
                
                if (params.count("page_num") > 0){
                    pageNum = stoi(params.at("page_num"));
                }
            }

            list(search, sort, pageNum);
    }
}

void UsersUI::list(string search, Sort sort, int pageNum, int maxPerPage){
    Users users;

    vector<Attribute> attributes = {
            {"name", "Name"},
            {"role", "Role"},
            {"status", "Status"},
            {"last_logged_in", "Last Logged In"}
        };

    string columnKey = ((sort.columnIndex >= 0) ? attributes[sort.columnIndex].key : "");
    TableList userList = users.userList(search, columnKey, sort.ascendingOrder, pageNum, maxPerPage);
    map<int, int> mappedIDs;

    // Mapping the id into input command (1-25)
    int i = 1;
    for (auto& row : userList.list){
        mappedIDs[i] = stoi(row.at("id"));
        i++;
    }

    UIManager::dataTable("Users List", attributes, userList.list, userList.totalRows, search, sort, pageNum, maxPerPage);

    cout<<"[A] Add New User"<<endl;
    cout<<"\n[X] Back"<<endl;

    // Commands
    cout<<"\nSelect Option > ";
    string input = UIManager::checkPresetInput();

    if (isInteger(input) && mappedIDs.count(stoi(input)) > 0){ // View Details
        UIManager::clearParams();
        UIManager::addParam("id", to_string(mappedIDs.at(stoi(input))));
        UIManager::goTo(2, 1);
    }
    else if (toUpperCase(input) == "P"){ // Previous
        int prevPage = pageNum - 1;
        prevPage = (prevPage < 1) ? 1 : prevPage;

        UIManager::addParam("page_num", to_string(prevPage));
    }
    else if (toUpperCase(input) == "N"){ // Next
        int nextPage = pageNum + 1;

        int lastPage = (userList.list.size() + maxPerPage - 1) / maxPerPage;
        lastPage = (lastPage == 0) ? 1 : lastPage;
        nextPage = (nextPage > lastPage) ? lastPage : nextPage;

        UIManager::addParam("page_num", to_string(nextPage));
    }
    else if (toUpperCase(input) == "R"){ // Reset
        UIManager::clearParams();
    }
    else if (toUpperCase(input) == "S"){ // Search
        cout<<"[X] Cancel"<<endl;
        cout<<"\nSearch > ";
        getline(cin, input);

        if (toUpperCase(input) == "X"){
            // Re-render the UI
        }
        else{
            UIManager::addParam("search", input);
        }
    }
    else if (toUpperCase(input) == "O"){ // Sort
        cout<<"[1] Sort by Name (Ascending)             [2] Sort by Name (Descending)"<<endl;
        cout<<"[3] Sort by Role (Ascending)             [4] Sort by Role (Descending)"<<endl;
        cout<<"[5] Sort by Status (Ascending)           [6] Sort by Status (Descending)"<<endl;
        cout<<"[7] Sort by Last Logged In (Ascending)   [8] Sort by Last Logged In (Descending)";
        cout<<"\n[X] Cancel"<<endl;

        cout<<"\nSelect Option > ";
        getline(cin, input);

        if (isInteger(input)){
            int option = stoi(input);
            switch (option) {
                case 1:
                case 2:
                    UIManager::addParam("sort_column_index", "0");
                    UIManager::addParam("sort_asc", ((option == 1) ? "true" : "false"));
                    break;

                case 3:
                case 4:
                    UIManager::addParam("sort_column_index", "1");
                    UIManager::addParam("sort_asc", ((option == 3) ? "true" : "false"));
                    break;

                case 5:
                case 6:
                    UIManager::addParam("sort_column_index", "2");
                    UIManager::addParam("sort_asc", ((option == 5) ? "true" : "false"));
                    break;

                case 7:
                case 8:
                    UIManager::addParam("sort_column_index", "3");
                    UIManager::addParam("sort_asc", ((option == 7) ? "true" : "false"));
                    break;

                default:
                    UIManager::addErrorMessage("Invalid Option.");
                    UIManager::addPresetInput("O");
            }
        }
        else if (toUpperCase(input) == "X"){
            // Re-render the UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
            UIManager::addPresetInput("O");
        }
    }
    else if (toUpperCase(input) == "A"){ // Register new user
        UIManager::goTo(2, 2);
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

void UsersUI::details(int id){
    Users users;

    Users::UserDetails details = users.userDetails(id);

    cout<<"\nUser Details"<<endl;
    cout<<string(UIManager::getlineLength(), '-')<<endl;
    if (details.id == -1){
        cout<<"User not Found."<<endl;
    }
    else{
        cout<<"Name           : "<<details.name<<endl;
        cout<<"Username       : "<<details.username<<endl;
        cout<<"Role           : "<<details.role<<endl;
        cout<<"Status         : "<<details.status<<endl;
        cout<<"Registered By  : "<<details.registeredByName<<endl;
        cout<<"Registered At  : "<<details.registeredAt<<endl;
        cout<<"Last Logged In : "<<details.lastLoggedIn<<endl;
    }
    cout<<string(UIManager::getlineLength(), '-')<<endl;

    cout<<"Actions:"<<endl;
    if (details.id != -1){
        cout<<"[U] Update Role"<<endl;
        if (toLowerCase(details.status) == "active"){
            cout<<"[D] Deactivate"<<endl;
        }
        else{
            cout<<"[A] Activate"<<endl;
        }
        cout<<"[R] Reset Password"<<endl<<endl;
    }
    cout<<"[X] Back"<<endl;

    cout<<"\nOption > ";
    string input = UIManager::checkPresetInput();

    if (toUpperCase(input) == "U"){ // Update Role
        cout<<"\nAvailable Roles: "<<endl;
        cout<<"   [1] Administrator"<<endl;
        cout<<"   [2] Employee"<<endl<<endl;
        cout<<"[X] Cancel"<<endl;

        cout<<"\nSelect Option > ";
        getline(cin, input);

        if (input == "1" || input == "2"){
            if (users.updateRole(details.id, ((input == "1") ? "Admin" : "Employee"))){
                UIManager::addInfoMessage("User's role has been successfully updated.");
            }
            else{
                UIManager::addErrorMessage("Failed to update the user's role.");
            }
        }
        else if (toUpperCase(input) == "X"){
            // Re-render the UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
            UIManager::addPresetInput("U");
        }
    }
    else if (toUpperCase(input) == "D" && toLowerCase(details.status) == "active"){ // Deactivate
        cout<<"\nAre you sure you want to deactivate this user?"<<endl;
        cout<<"Select Option (Y/N) > ";
        getline(cin, input);

        if (toUpperCase(input) == "Y"){
            if (users.deactivate(details.id)){
                UIManager::addInfoMessage("The user has been successfully deactivated.");
            }
            else{
                UIManager::addErrorMessage("Failed to deactivate the user.");
            }
        }
        else if (toUpperCase(input) == "N"){
            // Re-render the UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
            UIManager::addPresetInput("D");
        }
    }
    else if (toUpperCase(input) == "A" && toLowerCase(details.status) == "inactive"){ // Activate
        cout<<"\nAre you sure you want to activate this user?"<<endl;
        cout<<"Select Option (Y/N) > ";
        getline(cin, input);

        if (toUpperCase(input) == "Y"){
            if (users.activate(details.id)){
                UIManager::addInfoMessage("The user has been successfully activated.");
            }
            else{
                UIManager::addErrorMessage("Failed to activate the user.");
            }
        }
        else if (toUpperCase(input) == "N"){
            // Re-render the UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
            UIManager::addPresetInput("A");
        }
    }
    else if (toUpperCase(input) == "R"){ // Reset Password
        cout<<"\nAre you sure you want to reset this user's password?"<<endl;
        cout<<"Select Option (Y/N) > ";

        getline(cin, input);

        if (toUpperCase(input) == "Y"){
            if (users.resetPassword(details.id)){
                UIManager::addInfoMessage("The user's password has been successfully reset.");
            }
            else{
                UIManager::addErrorMessage("Failed to reset the user's password.");
            }
        }
        else if (toUpperCase(input) == "N"){
            // Re-render the UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
            UIManager::addPresetInput("R");
        }
    }
    else if (toUpperCase(input) == "X"){ // Back
        UIManager::clearParams();
        UIManager::clearPresetInputs();
        UIManager::goTo(2);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}

void UsersUI::registerUser(){
    Users users;

    cout<<"\nAdd New User"<<endl;
    cout<<string(UIManager::getlineLength(), '-')<<endl;

    string name, role, input;
    cout<<"Name : "<<endl;
    cout<<"> ";
    getline(cin, name);

    cout<<"\nRole : "<<endl;
    cout<<"   [1] Administrator"<<endl;
    cout<<"   [2] Employee"<<endl;
    cout<<"> ";
    getline(cin, role);

    cout<<endl<<string(UIManager::getlineLength(), '-')<<endl;
    cout<<"Actions:"<<endl;
    cout<<"[S] Submit"<<endl;
    cout<<"[C] Clear All"<<endl;
    cout<<"[X] Back"<<endl;

    cout<<"\nSelect Option > ";
    getline(cin, input);

    if (toUpperCase(input) == "S"){ // Submit
        bool isValid = true;
        if (!Validation::validateName(name)){
            UIManager::addErrorMessage("Name must be between 3 and 300 characters.");
            isValid = false;
        }

        if (role != "1" && role != "2"){
            UIManager::addErrorMessage("Invalid Option.");
            isValid = false;
        }

        if (isValid){
            if (users.registerUser(name, ((role == "1") ? "Admin" : "Employee"))){
                UIManager::addInfoMessage("New user has been successfully added.");
                UIManager::clearParams();
                UIManager::goTo(2);
            }
            else{
                UIManager::addErrorMessage("Failed to register new user.");
            }
        }
    }
    else if (toUpperCase(input) == "C"){ // Clear All
        // Re-render the UI
    }
    else if (toUpperCase(input) == "X"){ // Back
        UIManager::goTo(2);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}