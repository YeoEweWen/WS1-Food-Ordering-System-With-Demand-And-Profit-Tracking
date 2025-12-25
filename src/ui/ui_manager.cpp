#include "ui/ui_manager.h"
#include "core/auth.h"
#include "utils/functions.h"

using namespace std;

Page UIManager::currentPage = {0, 0, {}, {}};
int UIManager::lineLength = 120;

// ---------- Pages & Navigation ----------
Page UIManager::currentPageDetails(){
    return currentPage;
}

void UIManager::goTo(int id, int subID){
    currentPage.id = id;
    currentPage.subID = subID;
}

void UIManager::clearParams(){
    currentPage.params = {};
}

void UIManager::clearErrorMessages(){
    currentPage.errorMessages = {};
}

void UIManager::addParam(string key, string value){
    currentPage.params[key] = value;
}

void UIManager::addErrorMessage(string errorMessage){
    currentPage.errorMessages.push_back(errorMessage);
}


// ---------- Components ----------
void UIManager::header(const string& pageName){
    system("cls");

    int space;
    string storeName = "Ahmad Cafe";

    cout<<string(lineLength, '-')<<endl<<endl;
    space = (lineLength-11-countStringLength(storeName))/2;
    cout<<string(space, ' ')<<"Welcome to "<<storeName<<endl<<endl;
    cout<<string(lineLength, '-')<<endl;

    string firstName = "";

    if (Auth::isLoggedIn()){
        string name = Auth::retrieveLoggedUserDetails().name;
        stringstream ss(name);
        ss >> firstName;
        firstName = "Hi, " + firstName;
    }

    space = lineLength - countStringLength(pageName) - countStringLength(firstName);
    cout<<toUpperCase(pageName)<<string(space, ' ')<<firstName<<endl;
    cout<<string(lineLength, '-')<<endl;
}

TableNavPermission UIManager::table(vector<Attribute> columns, vector<map<string, string>> rows, int totalRows, string search, Sort sort, int pageNum, int maxRowPerPage){
    vector<int> ids = {};
    int numOfRows, lastPage;

    numOfRows = rows.size();
    lastPage = (numOfRows + maxRowPerPage - 1) / maxRowPerPage;

    // Calculate the maximum length per column
    int reservedSpace = 1;
    int reservedSpaceEachColumn = 2;
    int numOfColumns = columns.size();
    int availableSpace = lineLength - 6 - reservedSpace - (reservedSpaceEachColumn * numOfColumns);

    for (auto& column : columns){
        // Default
        column.maxLength = countStringLength(column.label);

        bool included = ids.empty();
        for (auto& row : rows){
            if (row.at(column.key) == "NULL"){ // Update null value
                row.at(column.key) = "-";
            }

            if (countStringLength(row.at(column.key)) > column.maxLength){
                column.maxLength = countStringLength(row.at(column.key));
            }

            // Add id into ids
            if (included){
                ids.push_back(stoi(row.at("id")));
            }
        }
        availableSpace -= column.maxLength;
    }

    availableSpace = availableSpace/numOfColumns;
    
    // Search & Filter
    cout<<endl;
    if (search != ""){
        cout<<"Search: "<<search<<endl;
    }
    if (sort.columnIndex != -1){
        cout<<"Sort: "<<columns[sort.columnIndex].label<<" ("<<((sort.ascendingOrder) ? "Ascending" : "Descending")<<")"<<endl;
    }

    // Print table
    int padding, balance, i, j;
    balance = lineLength - 6;

    // Header (Columns)
    cout<<string(lineLength, '-')<<endl;
    cout<<"|  # |";
    i = numOfColumns;
    for (auto& column : columns){
        i--;
        padding = (column.maxLength + availableSpace) - countStringLength(column.label);
        balance -= padding + countStringLength(column.label) + reservedSpaceEachColumn;
        padding = (i == 0) ? (padding + balance) : padding;

        cout<<" "<<column.label<<string(padding, ' ')<<"|";
    }
    cout<<endl;

    cout<<string(lineLength, '-')<<endl;

    // Contents (Rows)
    if (numOfRows > 0){
        i = 0;
        for (auto& row : rows){
            i++;
            j = numOfColumns;
            balance = lineLength - 6;

            cout<<"|"<<string(((i > 9) ? 1 : 2), ' ')<<i<<" |";

            for (auto& column : columns){
                j--;
                padding = (column.maxLength + availableSpace) - countStringLength(row.at(column.key));
                balance -= padding + countStringLength(row.at(column.key)) + reservedSpaceEachColumn;
                padding = (j == 0) ? (padding + balance) : padding;

                cout<<" "<<row.at(column.key)<<string(padding, ' ')<<"|";
            }
            cout<<endl;
        }
    }
    else{
        cout<<"|"<<string((lineLength-17)/2, ' ')<<"No record found."<<string((lineLength-17)/2, ' ')<<"|"<<endl;
    }
    cout<<string(lineLength, '-')<<endl;

    // Pagination
    string part1, part2;

    part1 = "Showing " + to_string((maxRowPerPage * (pageNum - 1)) + 1) + "-" + to_string((maxRowPerPage * (pageNum - 1)) + numOfRows) + " of " + to_string(totalRows) + " records";
    part2 = "Page " + to_string(pageNum) + "/" + to_string(lastPage);

    cout<<part1<<string((lineLength - countStringLength(part1 + part2)), ' ')<<part2<<endl;

    return {
        (pageNum != 1),
        (pageNum != lastPage),
        (lastPage != 1)
    };
}

void UIManager::errorMessages(const vector<string>& errorMessages, bool topBorder, bool bottomBorder){
    if (errorMessages.empty()){
        return;
    }

    if (topBorder){
        cout<<string(lineLength, '-')<<endl;
    }

    if (errorMessages.size() > 1){
        // Multi Line
        int i = 1;
        cout<<"Errors:"<<endl;
        for (const auto& msg : errorMessages){
            cout<<i<<". "<<msg<<endl;
            i++;
        }
    }
    else{
        // Single Line
        cout<<"Error: "<<errorMessages[0]<<endl;
    }
    
    if (bottomBorder){
        cout<<string(lineLength, '-')<<endl;
    }
}

void UIManager::emptyPage(bool includeHeader){
    if (includeHeader){
        header("empty page");
    }
    
    int space = (lineLength-19)/2;
    cout<<endl<<string(space, ' ')<<"This is empty page."<<endl<<endl;
    cout<<string(lineLength, '-')<<endl;

    system("pause"); // waits for a keypress
    exit(0);
}