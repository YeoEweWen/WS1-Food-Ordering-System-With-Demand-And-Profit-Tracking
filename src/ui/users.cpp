#include "ui/users.h"
#include "ui/ui_manager.h"
#include "core/users.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

void UsersUI::main() {
    Users users;

    TableList userList = users.userList();
    map<int, map<string, string>> mappedList;

    // Mapping the row to its own ID
    for (auto& row : userList.list){
        mappedList[stoi(row.at("id"))] = row;
    }

    UIManager::header("Users Management");

    TableNavPermission tableNavPermission = UIManager::table(
        {
            {"name", "Name"},
            {"role", "Role"},
            {"status", "Status"},
            {"last_logged_in", "Last Logged In"}
        },
        userList.list,
        userList.totalRows
    );

    if (tableNavPermission.previosPage){
        cout<<"[P] Previous   ";
    }
    if (tableNavPermission.nextPage){
        cout<<"[N] Next   ";
    }
    if (tableNavPermission.goToPage){
        cout<<"[G] Go To Page";
    }
    cout<<endl<<endl;

    cout<<endl<<endl<<endl;
    system("pause");
    exit(0);
}