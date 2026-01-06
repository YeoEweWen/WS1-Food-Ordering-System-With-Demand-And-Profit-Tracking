#include "ui/transaction_history.h"
#include "ui/ui_manager.h"
#include "core/order.h"
#include "core/auth.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

void TransactionHistoryUI::main() {
    Page page = UIManager::currentPageDetails();

    UIManager::header("Transaction History");
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
                UIManager::addErrorMessage("Invalid transaction's ID!");
                UIManager::goTo(5);
            }
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

void TransactionHistoryUI::list(string search, Sort sort, int pageNum, int maxPerPage){
    Order order;

    vector<Attribute> attributes = {
            {"created_at", "Created At"},
            {"created_by", "Created By"},
            {"transaction_status", "Status"},
            {"total_items", "Total Items"}
        };

    string columnKey = ((sort.columnIndex >= 0) ? attributes[sort.columnIndex].key : "");
    TableList transactionList = order.orderList(search, columnKey, sort.ascendingOrder, pageNum, maxPerPage);

    map<int, int> mappedIDs;

    // Mapping the id into input command (1-25)
    int i = 1;
    for (auto& row : transactionList.list){
        mappedIDs[i] = stoi(row.at("id"));
        i++;
    }

    UIManager::dataTable("Transaction List", attributes, transactionList.list, transactionList.totalRows, search, sort, pageNum, maxPerPage);

    cout<<"\n[X] Back"<<endl;

    // Commands
    string input;
    cout<<"\nSelect Option > ";
    getline(cin, input);
    cout<<endl;

    if (isInteger(input) && mappedIDs.count(stoi(input)) > 0){ // View Details
        UIManager::clearParams();
        UIManager::addParam("id", to_string(mappedIDs.at(stoi(input))));
        UIManager::goTo(5, 1);
    }
    else if (toUpperCase(input) == "P"){ // Previous
        int prevPage = pageNum - 1;
        prevPage = (prevPage < 1) ? 1 : prevPage;

        UIManager::addParam("page_num", to_string(prevPage));
    }
    else if (toUpperCase(input) == "N"){ // Next
        int nextPage = pageNum + 1;

        int lastPage = (transactionList.totalRows + maxPerPage - 1) / maxPerPage;
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
            UIManager::addParam("page_num", "1");
        }
    }
    else if (toUpperCase(input) == "O"){ // Sort
        cout<<"[1] Sort by Created At (Ascending)    [2] Sort by Created At (Descending)"<<endl;
        cout<<"[3] Sort by Created By (Ascending)    [4] Sort by Created By (Descending)"<<endl;
        cout<<"[5] Sort by Status (Ascending)        [6] Sort by Status (Descending)"<<endl;
        cout<<"[7] Sort by Total Items (Ascending)   [8] Sort by Total Items (Descending)"<<endl;
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
            }
        }
        else if (toUpperCase(input) == "X"){
            // Re-render the UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
        }
    }
    else if (toUpperCase(input) == "X"){ // Back
        UIManager::clearParams();
        UIManager::goTo(1);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}

void TransactionHistoryUI::details(int id){
    Order order;

    Order::OrderDetails details = order.orderDetails(id);

    cout<<"Transaction Details"<<endl;
    cout<<string(UIManager::getlineLength(), '-')<<endl;
    if (details.id == -1){
        cout<<"Transaction not Found."<<endl;
    }
    else{
        cout<<"Status         : "<<details.status<<endl;
        cout<<"Created By     : "<<details.createdBy<<endl;
        cout<<"Created At     : "<<details.createdAt<<endl;
        if (details.status == "Cancelled"){
            cout<<"Cancelled By   : "<<details.cancelledBy<<endl;
            cout<<"Cancelled At   : "<<details.cancelledAt<<endl;
        }

        UIManager::baseTable(
            "Items List",
            {
                {"name", "Item"},
                {"price", "Price", "RM "},
                {"quantity", "Quantity"},
                {"sub_total", "Sub Total", "RM "},
            },
            details.itemsList
        );

        string totalPrice = "Total: RM " + formatDecimalPoints(details.total, 2) + "   |";
        cout<<"|"<<string((UIManager::getlineLength() - countStringLength(totalPrice) - 1), ' ')<<totalPrice<<endl;
        cout<<string(UIManager::getlineLength(), '-')<<endl;
    }
    cout<<"Actions:"<<endl;
    if (details.id != -1){
        if (Auth::isAdmin()){
            if (details.status == "Completed"){
                cout<<"[C] Cancel Transaction"<<endl;
            }
            else{
                cout<<"[M] Mark as Completed"<<endl;
            }
        }
    }
    cout<<"[X] Back"<<endl;

    string input;
    cout<<"\nOption > ";
    input = UIManager::checkPresetInput();

    if (toUpperCase(input) == "C" && Auth::isAdmin() && details.status == "Completed"){ // Cancel Transaction
        cout<<"\nAre you sure you want to cancel this transaction?"<<endl;
        cout<<"Select Option (Y/N) > ";
        getline(cin, input);;

        if (toUpperCase(input) == "Y"){
            if (order.cancelOrder(details.id)){
                UIManager::addInfoMessage("The transaction has been successfully cancelled.");
                UIManager::goTo(5);
            }
            else{
                UIManager::addErrorMessage("Failed to cancel the transaction.");
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
    else if (toUpperCase(input) == "M" && Auth::isAdmin() && details.status == "Cancelled"){ // Mark as Completed
        cout<<"\nAre you sure you want to mark this transaction as completed?"<<endl;
        cout<<"Select Option (Y/N) > ";
        getline(cin, input);;

        if (toUpperCase(input) == "Y"){
            if (order.markOrderAsCompleted(details.id)){
                UIManager::addInfoMessage("The transaction has been successfully marked as completed.");
                UIManager::goTo(5);
            }
            else{
                UIManager::addErrorMessage("Failed to mark the transaction as completed.");
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
        UIManager::goTo(5);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}
