#include "ui/ui_manager.h"
#include "core/auth.h"
#include "utils/functions.h"

using namespace std;

Page UIManager::page = {0, 0, {}, {}};
int UIManager::lineLength = 120;

// ---------- Pages & Navigation ----------
int UIManager::getlineLength(){
    return lineLength;
}

Page UIManager::currentPageDetails(){
    return page;
}

void UIManager::goTo(int id, int subID){
    page.id = id;
    page.subID = subID;
}

void UIManager::clearParams(){
    page.params = {};
}

void UIManager::clearErrorMessages(){
    page.errorMessages = {};
}

void UIManager::clearInfoMessages(){
    page.infoMessages = {};
}

void UIManager::clearPresetInputs(){
    page.presetInputs = {};
}

void UIManager::addParam(string key, string value){
    page.params[key] = value;
}

void UIManager::addErrorMessage(string errorMessage){
    page.errorMessages.push_back(errorMessage);
}

void UIManager::addInfoMessage(string infoMessage){
    page.infoMessages.push_back(infoMessage);
}

void UIManager::addPresetInput(std::string input){
    page.presetInputs.push_back(input);
}

string UIManager::checkPresetInput(){
    string input;
    if (page.presetInputs.empty()){
        getline(cin, input); // No Preset Inputs
    }
    else{
        input = page.presetInputs[0];
        cout<<input<<endl;
        page.presetInputs.erase(page.presetInputs.begin());
    }

    return input;
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

void UIManager::baseTable(string title, vector<Attribute> columns, vector<map<string, string>> rows, string noRecordMessage){
    // Calculate the maximum length per column
    int reservedSpace = 1;
    int reservedSpaceEachColumn = 2;
    int numOfColumns = columns.size();
    int availableSpace = lineLength - 6 - reservedSpace - (reservedSpaceEachColumn * numOfColumns);
    int columnSize;

    for (auto& column : columns){
        // Default
        column.maxLength = countStringLength(column.label);

        for (auto& row : rows){
            if (row.at(column.key) == "NULL"){ // Update null value
                row.at(column.key) = "-";
            }

            columnSize = countStringLength(row.at(column.key)) + countStringLength(column.additionalFrontLabel) + countStringLength(column.additionalBackLabel);
            if (columnSize > column.maxLength){
                column.maxLength = columnSize;
            }
        }
        availableSpace -= column.maxLength;
    }

    availableSpace = availableSpace/numOfColumns;

    // Print table
    int padding, balance, i, j;
    balance = lineLength - 6;

    // Title
    if (title != ""){
        cout<<string(((lineLength - countStringLength(title)) / 2), ' ')<<title<<endl;
    }

    // Header (Columns)
    cout<<string(lineLength, '-')<<endl;
    cout<<"|  # |";
    i = numOfColumns;
    for (auto& column : columns){
        i--;
        padding = (column.maxLength + availableSpace) - countStringLength(column.label);
        columnSize = countStringLength(column.label) + countStringLength(column.additionalFrontLabel) + countStringLength(column.additionalBackLabel);
        balance -= padding + columnSize + reservedSpaceEachColumn;
        padding = (i == 0) ? (padding + balance) : padding;
        padding += countStringLength(column.additionalFrontLabel + column.additionalBackLabel);

        cout<<" "<<column.label<<string(padding, ' ')<<"|";
    }
    cout<<endl;

    cout<<string(lineLength, '-')<<endl;

    // Contents (Rows)
    int numOfRows = rows.size();
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
                columnSize = countStringLength(row.at(column.key)) + countStringLength(column.additionalFrontLabel) + countStringLength(column.additionalBackLabel);
                balance -= padding + columnSize + reservedSpaceEachColumn;
                padding = (j == 0) ? (padding + balance) : padding;

                cout<<" "<<column.additionalFrontLabel<<row.at(column.key)<<column.additionalBackLabel<<string(padding, ' ')<<"|";
            }
            cout<<endl;
        }
    }
    else{
        cout<<"|"<<string((lineLength-2-countStringLength(noRecordMessage))/2, ' ')<<noRecordMessage<<string((lineLength-2-countStringLength(noRecordMessage))/2, ' ')<<"|"<<endl;
    }
    cout<<string(lineLength, '-')<<endl;
}

void UIManager::dataTable(string title, vector<Attribute> columns, vector<map<string, string>> rows, int totalRows, string search, Sort sort, int pageNum, int maxRowPerPage){
    int numOfRows, lastPage;

    numOfRows = rows.size();
    lastPage = (numOfRows + maxRowPerPage - 1) / maxRowPerPage;
    lastPage = (lastPage == 0) ? 1 : lastPage;

    // Search & Filter
    cout<<endl;
    if (search != ""){
        cout<<"Search: "<<search<<endl;
    }
    if (sort.columnIndex != -1){
        cout<<"Sort: "<<columns[sort.columnIndex].label<<" ("<<((sort.ascendingOrder) ? "Ascending" : "Descending")<<")"<<endl;
    }

    // Table
    baseTable(title, columns, rows);

    // Pagination
    string part1, part2;

    part1 = "Showing " + ((numOfRows > 0) ? to_string((maxRowPerPage * (pageNum - 1)) + 1) : "0") + "-" + to_string((maxRowPerPage * (pageNum - 1)) + numOfRows) + " of " + to_string(totalRows) + " records";
    part2 = "Page " + to_string(pageNum) + "/" + to_string(lastPage);

    cout<<part1<<string((lineLength - countStringLength(part1 + part2)), ' ')<<part2<<endl;

    cout<<endl<<"Actions:"<<endl;
    if (numOfRows > 0){
        cout<<"[1"<<((numOfRows > 1) ? "-" + to_string(numOfRows) : "")<<"] View Details"<<endl;
    }
    if (pageNum != 1){
        cout<<"[P] Previous   ";
    }
    if (pageNum != lastPage){
        cout<<"[N] Next   ";
    }
    if (pageNum != 1 || pageNum != lastPage){
        cout<<endl;
    }
    cout<<"[S] Search   [O] Sort"<<endl;
    cout<<"[R] Reset"<<endl;
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

void UIManager::infoMessages(const vector<string>& infoMessages, bool topBorder, bool bottomBorder){
    if (infoMessages.empty()){
        return;
    }

    if (topBorder){
        cout<<string(lineLength, '-')<<endl;
    }

    if (infoMessages.size() > 1){
        // Multi Line
        int i = 1;
        cout<<"Info:"<<endl;
        for (const auto& msg : infoMessages){
            cout<<i<<". "<<msg<<endl;
            i++;
        }
    }
    else{
        // Single Line
        cout<<"Info: "<<infoMessages[0]<<endl;
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