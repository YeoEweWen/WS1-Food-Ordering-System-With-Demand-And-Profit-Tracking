#include "ui/users.h"
#include "ui/ui_manager.h"
#include "core/users.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

void UsersUI::main() {
    Page page = UIManager::currentPageDetails();

    UIManager::header("Users Management");
    UIManager::errorMessages(page.errorMessages);

    map<string, string> params = page.params;

    UIManager::clearErrorMessages();

    switch (page.subID){
        case 1:
            logInfo("This is view details");
            UIManager::emptyPage(false);
            break;
        
        case 2:
            logInfo("This is update role");
            UIManager::emptyPage(false);
            break;

        case 3:
            logInfo("This is register user");
            UIManager::emptyPage(false);
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

    UIManager::dataTable(attributes, userList.list, userList.totalRows, search, sort, pageNum, maxPerPage);

    cout<<"[N] Register New User"<<endl;
    cout<<endl<<"[X] Back"<<endl;

    // Commands
    string input;
    cout<<"\nCommand > ";
    cin>>input;

    if (isInteger(input) && mappedIDs.count(stoi(input)) > 0){ // View Details
        UIManager::clearParams();
        UIManager::addParam("id", to_string(mappedIDs.at(stoi(input))));
        UIManager::goTo(2, 1);
    }
    else if (toProperCase(input) == "P"){ // Previous

        UIManager::goTo(2);
    }
    else if (toProperCase(input) == "N"){ // Next

        UIManager::goTo(2);
    }
    else if (toProperCase(input) == "G"){ // Go To Page

        UIManager::goTo(2);
    }
    else if (toProperCase(input) == "R"){ // Reset

        UIManager::goTo(2);
    }
    else if (toProperCase(input) == "S"){ // Search

        UIManager::goTo(2);
    }
    else if (toProperCase(input) == "O"){ // Sort

        UIManager::goTo(2);
    }
    else if (toProperCase(input) == "N"){ // Register new user
        UIManager::clearParams();
        UIManager::goTo(2, 3);
    }
    else if (toProperCase(input) == "X"){ // Back
        UIManager::clearParams();
        UIManager::goTo(1);
    }
    else{
        UIManager::addErrorMessage("Invalid command.");
    }
}

void UsersUI::details(int id){

}

void UsersUI::registerUser(){

}

void UsersUI::updateRole(){

}