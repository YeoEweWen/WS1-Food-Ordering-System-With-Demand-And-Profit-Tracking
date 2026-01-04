#include "ui/order.h"
#include "ui/ui_manager.h"
#include "core/menu.h"
#include "core/order.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

map<int, map<string, string>> OrderUI::mappedMenuPriceList = {};
map<int, string> OrderUI::categoryList = {};
map<int, vector<int>> OrderUI::categorizedMenuPriceList = {};

void OrderUI::main() {
    Page page = UIManager::currentPageDetails();

    UIManager::header("Order System");
    UIManager::errorMessages(page.errorMessages, false);
    UIManager::infoMessages(page.infoMessages, false);

    map<string, string> params = page.params;

    UIManager::clearErrorMessages();
    UIManager::clearInfoMessages();

    Menu menu;
    Order order;

    int i, j;
    if (mappedMenuPriceList.empty()){
        // Retrieve menu list from db
        map<string, int> registeredCategory;
        vector<map<string, string>> menuPriceList = menu.menuPriceList();

        int categoryIndex;
        string category;

        i = 1;
        j = 0;
        for (const auto& menu : menuPriceList){
            category = menu.at("category");
            if (registeredCategory.find(category) == registeredCategory.end()){
                // Category not registered yet
                categoryList[i] = category;
                registeredCategory[category]  = i;
                categoryIndex = i;
                i++;
            }
            else{
                // Category already registered
                categoryIndex = registeredCategory[category];
            }

            categorizedMenuPriceList[categoryIndex].push_back(j);
            mappedMenuPriceList[j] = {
                {"id", menu.at("id")},
                {"name", menu.at("name")},
                {"production_cost", menu.at("production_cost")},
                {"selling_price", menu.at("selling_price")},
            };

            j++;
        }
    }

    map<int, Order::Item> mappedItems;
    vector<map<string, string>> rows = {};
    vector<Order::Item> itemsList = order.itemsList();
    double total = 0;

    i = 1;
    for (const auto& item : itemsList){
        rows.push_back(
            {
                {"item", item.name},
                {"price", formatDecimalPoints(item.sellingPrice, 2)},
                {"quantity", to_string(item.quantity)},
                {"sub_total", formatDecimalPoints((item.sellingPrice * item.quantity), 2)},
            }
        );

        mappedItems[i] = item;
        total += item.sellingPrice * item.quantity;
        i++;
    }

    UIManager::baseTable(
        "Cart",
        {
            {"item", "Item"},
            {"price", "Price", "RM "},
            {"quantity", "Quantity"},
            {"sub_total", "Sub Total", "RM "}
        },
        rows,
        "The cart is empty."
    );
    string totalPrice = "Total: RM " + formatDecimalPoints(total, 2) + "   |";
    cout<<"|"<<string((UIManager::getlineLength() - countStringLength(totalPrice) - 1), ' ')<<totalPrice<<endl;
    cout<<string(UIManager::getlineLength(), '-')<<endl;


    cout<<endl<<"Actions:"<<endl;
    int numOfRows = rows.size();
    if (numOfRows > 0){
        cout<<"[U] Update Quantity"<<endl;
        cout<<"[R] Remove Item"<<endl;
        cout<<"[C] Clear Cart"<<endl;
        cout<<"[S] Submit Order"<<endl;
    }
    cout<<"[A] Add New Item"<<endl;
    cout<<"[X] Back"<<endl;

    // Commands
    cout<<"\nSelect Option > ";
    string input = UIManager::checkPresetInput();

    if ((toUpperCase(input) == "U" || toUpperCase(input) == "R") && (numOfRows > 0)){ // Update quantity & remove item
        cout<<"Select Item "<<"[1"<<((numOfRows > 1) ? "-" + to_string(numOfRows) : "")<<"] > ";
        string selectedItem;
        getline(cin, selectedItem);

        if (isInteger(selectedItem) && stoi(selectedItem) > 0 && stoi(selectedItem) <= numOfRows){
            Order::Item item = order.itemsList()[(stoi(selectedItem) - 1)];
            
            if (toUpperCase(input) == "U"){ // Update quantity
                string quantity;
                cout<<"New Quantity > ";
                getline(cin, quantity);

                if (isInteger(quantity) && stoi(quantity) > 0){
                    order.updateQuantity(item.id, stoi(quantity));
                    UIManager::addInfoMessage(item.name + "'s quantity has been successful updated.");
                }
                else if (isInteger(quantity)){
                    UIManager::addErrorMessage("Invalid quantity format.");
                }
                else{
                    UIManager::addErrorMessage("Minimum quatity is 1.");
                }
            }
            else{ // Remove Item
                cout<<"\nAre you sure you want to remove "<<item.name<<" from the cart?"<<endl;
                cout<<"Select Option (Y/N) > ";
                getline(cin, input);

                if (toUpperCase(input) == "Y"){
                    order.removeItem(item.id);
                    UIManager::addInfoMessage(item.name + " has been successfully removed.");
                }
                else if (toUpperCase(input) == "N"){
                    // Re-render the UI
                }
                else{
                    UIManager::addErrorMessage("Invalid Option.");
                    UIManager::addPresetInput("R");
                }
            }
        }
        else{
            UIManager::addErrorMessage("Invalid Item.");
        }
    }
    else if (toUpperCase(input) == "C" && (numOfRows > 0)){ // Clear Cart
        cout<<"\nAre you sure you want to clear the cart?"<<endl;
        cout<<"Select Option (Y/N) > ";
        getline(cin, input);

        if (toUpperCase(input) == "Y"){
            order.clearItems();
            UIManager::addInfoMessage("Cart has been successful cleared.");
        }
        else if (toUpperCase(input) == "N"){
            // Re-render the UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
        }
    }
    else if (toUpperCase(input) == "S" && (numOfRows > 0)){ // Submit Order
        cout<<"\nAre you sure you want to submit the order?"<<endl;
        cout<<"Select Option (Y/N) > ";
        getline(cin, input);

        if (toUpperCase(input) == "Y"){
            if (order.createOrder()){
                UIManager::addInfoMessage("Order has been successfully submitted.");
                order.clearItems();
            }
            else{
                UIManager::addErrorMessage("Failed to submit order.");
            }
        }
        else if (toUpperCase(input) == "N"){
            // Re-render the UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
        }
    }
    else if (toUpperCase(input) == "A"){ // Add New Menu
        // Category
        cout<<"\nCategory: "<<endl;
        for (const auto& [index, category] : categoryList){
            cout<<"   ["<<index<<"] "<<category<<endl;
        }
        cout<<"\n[X] Cancel"<<endl;

        cout<<"\nSelect Category > ";
        string category;
        getline(cin, category);

        if (isInteger(category) && categoryList.find(stoi(category)) != categoryList.end()){
            // Menu
            cout<<"\nMenu: "<<endl;
            i = 1;
            map<int, int> mappedMenu;
            for (const auto& index : categorizedMenuPriceList.at(stoi(category))){
                mappedMenu[i] = index;
                cout<<"   ["<<i<<"] "<<mappedMenuPriceList.at(index).at("name")<<"  RM "<<mappedMenuPriceList.at(index).at("selling_price")<<endl;
                i++;
            }

            string menu, quantity;
            
            cout<<"\nSelect Menu > ";
            getline(cin, menu);

            if (!(isInteger(menu) && stoi(menu) > 0 && stoi(menu) <= categorizedMenuPriceList.at(stoi(category)).size())){
                UIManager::addErrorMessage("Invalid menu option.");
            }
            else{
                cout<<"Quantity > ";
                getline(cin, quantity);

                bool isValid = true;
                if (!isInteger(quantity)){
                    UIManager::addErrorMessage("Invalid quantity format.");
                    isValid = false;
                }
                else if (stoi(quantity) < 0){
                    UIManager::addErrorMessage("Minimum quatity is 1.");
                    isValid = false;
                }

                if (isValid){
                    map<string, string> menuDetails = mappedMenuPriceList.at(mappedMenu.at(stoi(menu)));
                    order.addItem(stoi(menuDetails.at("id")), menuDetails.at("name"), stod(menuDetails.at("production_cost")), stod(menuDetails.at("selling_price")), stoi(quantity));
                    UIManager::clearPresetInputs();
                }
            }
        }
        else if (toUpperCase(category) == "X"){ // Back
            // Re-render UI
        }
        else{
            UIManager::addErrorMessage("Invalid Option.");
            UIManager::addPresetInput("A");
        }
    }
    else if (toUpperCase(input) == "X"){ // Back
        UIManager::clearParams();
        UIManager::clearPresetInputs();
        clearStoredMenuAndCart();
        UIManager::goTo(1);
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}

void OrderUI::clearStoredMenuAndCart(){
    Order order;

    mappedMenuPriceList = {};
    categoryList = {};
    categorizedMenuPriceList = {};

    order.clearItems();
}
