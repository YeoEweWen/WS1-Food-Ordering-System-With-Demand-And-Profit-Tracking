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

    switch (page.subID){
        case 1:
            break;
        
        case 2:
            break;

        case 3:
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
    map<int, map<string, string>> mappedList;

    // Mapping the row to its own ID
    for (auto& row : userList.list){
        mappedList[stoi(row.at("id"))] = row;
    }

    TableNavPermission tableNavPermission = UIManager::table(attributes, userList.list, userList.totalRows, search, sort, pageNum, maxPerPage);

    cout<<endl<<"Actions:"<<endl;
    cout<<"[1-25] View Details"<<endl; // Put this in table
    if (tableNavPermission.previosPage){
        cout<<"[P] Previous   ";
    }
    if (tableNavPermission.nextPage){
        cout<<"[N] Next   ";
    }
    if (tableNavPermission.goToPage){
        cout<<"[G] Go To Page   ";
    }
    cout<<"[R] Reset"<<endl;
    cout<<"[S] Search   [O] Sort"<<endl;
    cout<<"[N] Register New User"<<endl;
    cout<<endl<<"[X] Back"<<endl;

    string input;
    cout<<"\nCommand > ";
    cin>>input;
}

void UsersUI::details(int id){

}

void UsersUI::registerUser(){

}

void UsersUI::updateRole(){

}