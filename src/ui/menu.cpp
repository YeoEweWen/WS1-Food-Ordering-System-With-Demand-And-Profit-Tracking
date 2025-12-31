#include "ui/menu.h"
#include "ui/ui_manager.h"
#include "core/menu.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

void MenuUI::main() {
    Page page = UIManager::currentPageDetails();

    UIManager::header("Menu Management");
    UIManager::errorMessages(page.errorMessages, false);
    UIManager::infoMessages(page.infoMessages, false);

    map<string, string> params = page.params;

    UIManager::clearErrorMessages();
    UIManager::clearInfoMessages();

    switch (page.subID){
        case 1:
            if (!params.empty() && params.count("id") > 0){
                menuDetails(stoi(params.at("id")));
            }
            else{
                UIManager::addErrorMessage("Invalid Menu's ID!");
                UIManager::goTo(3);
            }
            break;

        case 2:
            addMenu();
            break;
        
        case 3:
            manageCategory();
            break;

        case 4:
            if (!params.empty() && params.count("id") > 0){
                categoryDetails(stoi(params.at("id")));
            }
            else{
                UIManager::addErrorMessage("Invalid Category's ID!");
                UIManager::goTo(3, 3);
            }
            break;

        case 5:
            addCategory();
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

void MenuUI::list(string search, Sort sort, int pageNum, int maxPerPage){
    Menu menu;

    vector<Attribute> attributes = {
            {"name", "Name"},
            {"price", "Price", "RM"},
            {"category", "Category"},
            {"availability", "Availability"}
        };

    string columnKey = ((sort.columnIndex >= 0) ? attributes[sort.columnIndex].key : "");
    TableList menuList = menu.menuList(search, columnKey, sort.ascendingOrder, pageNum, maxPerPage);
    map<int, int> mappedIDs;

    // Mapping the id into input command (1-25)
    int i = 1;
    for (auto& row : menuList.list){
        mappedIDs[i] = stoi(row.at("id"));
        i++;
    }

    UIManager::dataTable("Menu List", attributes, menuList.list, menuList.totalRows, search, sort, pageNum, maxPerPage);

    cout<<"[A] Add New Menu"<<endl;
    cout<<"[M] Manage Category"<<endl;
    cout<<"\n[X] Back"<<endl;

    // Commands
    cout<<"\nSelect Option > ";
    string input = UIManager::checkPresetInput();

    if (isInteger(input) && mappedIDs.count(stoi(input)) > 0){ // View Details
        UIManager::clearParams();
        UIManager::addParam("id", to_string(mappedIDs.at(stoi(input))));
        UIManager::goTo(3, 1);
    }
    else if (toUpperCase(input) == "P"){ // Previous
        int prevPage = pageNum - 1;
        prevPage = (prevPage < 1) ? 1 : prevPage;

        UIManager::addParam("page_num", to_string(prevPage));
    }
    else if (toUpperCase(input) == "N"){ // Next
        int nextPage = pageNum + 1;

        int lastPage = (menuList.list.size() + maxPerPage - 1) / maxPerPage;
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
        cout<<"[1] Sort by Name (Ascending)           [2] Sort by Name (Descending)"<<endl;
        cout<<"[3] Sort by Price (Ascending)          [4] Sort by Price (Descending)"<<endl;
        cout<<"[5] Sort by Category (Ascending)       [6] Sort by Category (Descending)"<<endl;
        cout<<"[7] Sort by Availability (Ascending)   [8] Sort by Availability (Descending)"<<endl;
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
    else if (toUpperCase(input) == "A"){ // Add New Menu
        UIManager::goTo(3, 2);
    }
    else if (toUpperCase(input) == "M"){ // Manage category
        UIManager::goTo(3, 3);
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

void MenuUI::menuDetails(int id){
    Menu menu;

    Menu::MenuDetails details = menu.menuDetails(id);

    cout<<"\nMenu Details"<<endl;
    cout<<string(UIManager::getlineLength(), '-')<<endl;
    if (details.id == -1){
        cout<<"Menu not Found."<<endl;
    }
    else{
        cout<<"Name             : "<<details.name<<endl;
        cout<<"Production Cost  : RM"<<fixed<<setprecision(2)<<details.productionCost<<endl;
        cout<<"Selling Price    : RM"<<fixed<<setprecision(2)<<details.sellingPrice<<endl;
        cout<<"Profit Margin    : "<<fixed<<setprecision(1)<<details.profitMargin<<"%"<<endl;
        cout<<"Category         : "<<details.category<<endl;
        cout<<"Type             : "<<details.type<<endl;
        cout<<"Availability     : "<<details.availability<<endl;
        cout<<"Added By         : "<<details.createdByName<<endl;
        cout<<"Added At         : "<<details.createdAt<<endl;
        cout<<"Last Update By   : "<<details.updatedByName<<endl;
        cout<<"Last Update At   : "<<details.updatedAt<<endl;
    }
    cout<<string(UIManager::getlineLength(), '-')<<endl;

    cout<<"Actions:"<<endl;
    if (details.id != -1){
        cout<<"[U] Update"<<endl;
        if (details.deletable){
            cout<<"[D] Delete"<<endl;
        }
        if (toLowerCase(details.availability) == "available"){
            cout<<"[S] Set as Unavailable"<<endl;
        }
        else{
            cout<<"[S] Set as Available"<<endl;
        }
    }
    cout<<"[X] Back"<<endl<<endl;
    
    string input;

    cout<<"\nOption > ";
    input = UIManager::checkPresetInput(); // Replace cin

    if (toUpperCase(input) == "U"){ // Update Name, Cost, Price, Category
        cout<<"\nUpdate: "<<endl;
        cout<<"   [1] Name"<<endl;
        cout<<"   [2] Production Cost & Selling Price"<<endl;
        cout<<"   [3] Category"<<endl<<endl;
        cout<<"[X] Cancel"<<endl;
        
        cout<<"\nSelect Option > ";
        input = UIManager::checkPresetInput();

        if (input == "1"){ // Update Name
            
        }
        else if (input == "2"){ // Update Production Cost & Selling Price
            
        }
        else if (input == "3"){ // Update Category
            
        }
        else if (toUpperCase(input) == "X"){
            // Re-render the UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
            UIManager::addPresetInput("U");
        }
    }
    else if (toUpperCase(input) == "D"){ // Delete
        cout<<"\nAre you sure you want to delete this menu?"<<endl;
        cout<<"Select Option (Y/N) > ";
        getline(cin, input);

        if (toUpperCase(input) == "Y"){
            if (menu.deleteMenu(details.id)){
                UIManager::addInfoMessage("The menu has been successfully deleted.");
            }
            else{
                UIManager::addErrorMessage("Failed to delete the menu.");
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
    else if (toUpperCase(input) == "S"){ // Set as Available/Unavailable
        cout<<"\nAre you sure you want to set this menu as "<<((toLowerCase(details.availability) == "available") ? "unavailable?" : "available?")<<endl;
        cout<<"Select Option (Y/N) > ";
        getline(cin, input);

        if (toUpperCase(input) == "Y"){
            bool result;

            if (toLowerCase(details.availability) == "available"){
                result = menu.setUnavailable(details.id);
            }
            else{
                result = menu.setAvailable(details.id);
            }

            if (result){
                UIManager::addInfoMessage(string("The menu has been successfully set as ") + ((toLowerCase(details.availability) == "available") ? "unavailable." : "available."));
            }
            else{
                UIManager::addErrorMessage(string("Failed to set the menu as ") + ((toLowerCase(details.availability) == "available") ? "unavailable." : "available."));
            }
        }
        else if (toUpperCase(input) == "N"){
            // Re-render the UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
            UIManager::addPresetInput("S");
        }
    }
    else if (toUpperCase(input) == "X"){ // Back
        UIManager::clearParams();
        UIManager::clearPresetInputs();
        UIManager::goTo(3);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
        UIManager::clearPresetInputs();
    }
}

void MenuUI::addMenu(){



    /*
    Users users;

    cout<<"Register New User"<<endl;
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
            logInfo("Name: " + name);
            UIManager::addErrorMessage("Name must be between 3 and 300 characters.");
            isValid = false;
        }

        if (role != "1" && role != "2"){
            UIManager::addErrorMessage("Invalid role.");
            isValid = false;
        }

        if (isValid){
            if (users.registerUser(name, ((role == "1") ? "Admin" : "Employee"))){
                UIManager::addInfoMessage("New user has been successfully added.");
                UIManager::clearParams();
                UIManager::goTo(3);
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
        UIManager::goTo(3);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }

    */
}

void MenuUI::manageCategory(){
    Menu menu;

    vector<Attribute> attributes = {
            {"category", "Name"},
            {"type", "Type"}
        };

    vector<map<string, string>> list = menu.categoryList();
    map<int, int> mappedIDs;

    // Mapping the id into input command
    int i = 1;
    for (auto& row : list){
        mappedIDs[i] = stoi(row.at("id"));
        i++;
    }

    UIManager::baseTable("Category List", attributes, list);

    cout<<endl<<"Actions:"<<endl;
    int numOfRows = list.size();
    if (numOfRows > 0){
        cout<<"[1"<<((numOfRows > 1) ? "-" + to_string(numOfRows) : "")<<"] View Details"<<endl;
    }
    cout<<"[A] Add New Category"<<endl;
    cout<<"\n[X] Back"<<endl;

    // Commands
    cout<<"\nSelect Option > ";
    string input = UIManager::checkPresetInput();

    if (isInteger(input) && mappedIDs.count(stoi(input)) > 0){ // View Details
        UIManager::clearParams();
        UIManager::addParam("id", to_string(mappedIDs.at(stoi(input))));
        UIManager::goTo(3, 4);
    }
    else if (toUpperCase(input) == "A"){ // Add New Category
        UIManager::goTo(3, 5);
    }
    else if (toUpperCase(input) == "X"){ // Back
        UIManager::clearParams();
        UIManager::clearPresetInputs();
        UIManager::goTo(3);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }

}

void MenuUI::categoryDetails(int id){
    Menu menu;

    Menu::CategoryDetails details = menu.categoryDetails(id);

    cout<<"\nCategory Details"<<endl;
    cout<<string(UIManager::getlineLength(), '-')<<endl;
    if (details.id == -1){
        cout<<"Category not Found."<<endl;
    }
    else{
        cout<<"Name             : "<<details.name<<endl;
        cout<<"Type             : "<<details.type<<endl;
        cout<<"Added By         : "<<details.createdByName<<endl;
        cout<<"Added At         : "<<details.createdAt<<endl;
        cout<<"Last Update By   : "<<details.updatedByName<<endl;
        cout<<"Last Update At   : "<<details.updatedAt<<endl;
    }
    cout<<string(UIManager::getlineLength(), '-')<<endl;

    cout<<"Actions:"<<endl;
    if (details.id != -1){
        cout<<"[U] Update"<<endl;
        if (details.deletable){
            cout<<"[D] Delete"<<endl;
        }
    }
    cout<<"[X] Back"<<endl;
    
    string input;

    cout<<"\nOption > ";
    input = UIManager::checkPresetInput(); // Replace cin

    if (toUpperCase(input) == "U"){ // Update Name, Type
        cout<<"\nUpdate: "<<endl;
        cout<<"   [1] Name"<<endl;
        cout<<"   [2] Type"<<endl;
        cout<<"[X] Cancel"<<endl;
        
        cout<<"\nSelect Option > ";
        input = UIManager::checkPresetInput();

        string name, type, selectedOption;

        if (input == "1" || input == "2"){ // Update
            selectedOption = input;
            if (input == "1"){ // Name
                cout<<"\nName : "<<endl;
                cout<<"> ";
                getline(cin, name);
            }
            else{ // Type
                cout<<"\nType : "<<endl;
                cout<<"   [1] Food"<<endl;
                cout<<"   [2] Beverage"<<endl;
                cout<<"> ";
                getline(cin, type);
            }
            
            cout<<endl<<string(UIManager::getlineLength(), '-')<<endl;
            cout<<"Actions:"<<endl;
            cout<<"[S] Submit"<<endl;
            cout<<"[C] Clear All"<<endl;
            cout<<"[X] Back"<<endl;

            cout<<"\nSelect Option > ";
            getline(cin, input);

            if (toUpperCase(input) == "S"){ // Submit
                if (selectedOption == "1"){
                    if (Validation::validateMenuCategoryName(name)){
                        if (menu.updateCategoryName(id, name)){
                            UIManager::addInfoMessage("Category name has been successfully updated.");
                            UIManager::clearPresetInputs();
                        }
                        else{
                            UIManager::addErrorMessage("Failed to update category name.");
                        }
                    }
                    else{
                        UIManager::addErrorMessage("Category name must be between 3 and 100 characters.");
                    }
                }
                else{
                    if (type != "1" || type != "2"){
                        if (menu.updateCategoryType(id, ((type == "1") ? "Food" : "Beverage"))){
                            UIManager::addInfoMessage("Category type has been successfully updated.");
                            UIManager::clearPresetInputs();
                        }
                        else{
                            UIManager::addErrorMessage("Failed to update category type.");
                        }
                    }
                    else{
                        UIManager::addErrorMessage("Invalid Option.");
                        UIManager::addPresetInput("U");
                        UIManager::addPresetInput(selectedOption);
                    }
                }
            }
            else if (toUpperCase(input) == "C"){ // Clear All
                UIManager::addPresetInput("U");
                UIManager::addPresetInput(selectedOption);
                // Re-render the UI
            }
            else if (toUpperCase(input) == "X"){ // Back
                UIManager::goTo(3, 4);
            }
            else{
                UIManager::addErrorMessage("Invalid Option.");
                UIManager::addPresetInput("U");
                UIManager::addPresetInput(selectedOption);
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
    else if (toUpperCase(input) == "D"){ // Delete
        cout<<"\nAre you sure you want to delete this category?"<<endl;
        cout<<"Select Option (Y/N) > ";
        getline(cin, input);

        if (toUpperCase(input) == "Y"){
            if (menu.deleteCategory(details.id)){
                UIManager::addInfoMessage("The category has been successfully deleted.");
            }
            else{
                UIManager::addErrorMessage("Failed to delete the category.");
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
    else if (toUpperCase(input) == "X"){ // Back
        UIManager::clearParams();
        UIManager::clearPresetInputs();
        UIManager::goTo(3, 3);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
        UIManager::clearPresetInputs();
    }
}

void MenuUI::addCategory(){
    Menu menu;

    cout<<"\nAdd New Category"<<endl;
    cout<<string(UIManager::getlineLength(), '-')<<endl;

    string name, type, input;
    cout<<"Name : "<<endl;
    cout<<"> ";
    getline(cin, name);

    cout<<"\nType : "<<endl;
    cout<<"   [1] Food"<<endl;
    cout<<"   [2] Beverage"<<endl;
    cout<<"> ";
    getline(cin, type);

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
            UIManager::addErrorMessage("Category name must be between 3 and 100 characters.");
            isValid = false;
        }

        if (type != "1" && type != "2"){
            UIManager::addErrorMessage("Invalid OPtion.");
            isValid = false;
        }

        if (isValid){
            if (menu.addCategory(name, ((type == "1") ? "Food" : "Beverage"))){
                UIManager::addInfoMessage("New category has been successfully added.");
                UIManager::clearParams();
                UIManager::goTo(3, 3);
            }
            else{
                UIManager::addErrorMessage("Failed to add new category.");
            }
        }
    }
    else if (toUpperCase(input) == "C"){ // Clear All
        // Re-render the UI
    }
    else if (toUpperCase(input) == "X"){ // Back
        UIManager::goTo(3, 3);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}