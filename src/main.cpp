// Core
#include "core/auth.h"
#include "core/menu.h"
#include "core/order.h"
#include "core/users.h"

// UIManager
#include "ui/auth.h"
#include "ui/main_menu.h"
#include "ui/users.h"
#include "ui/ui_manager.h"

// Utils
#include "utils/functions.h"

#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    Page page;

    /*
    id | page
    ----------------------
    0  | auth
    1  | dashboard (default)
    2  | users
    3  | menu
    4  | order
    5  | report
    6  | profile
    */

    // Bypass login for development
    Auth::LoginStatus loginStatus = Auth::login("ali7703", "p@ssW0rd"); 
    if (!loginStatus.success){
        cout<<loginStatus.description<<endl;
        system("pause");
        exit(0);
    }
    if (!loginStatus.usingPresetPassword){
        UIManager::goTo(1);
    }

    while (true){
        page = UIManager::currentPageDetails();

        if (!Auth::isLoggedIn()){
            AuthUI::login();
            continue;
        }

        switch (page.id){
            case 0:
                AuthUI::updatePassword();
                break;

            case 2:
                UsersUI::main();
                break;

            case 3:
                UIManager::emptyPage();
                break;

            case 4:
                UIManager::emptyPage();
                break;

            case 5:
                UIManager::emptyPage();
                break;

            case 6:
                UIManager::emptyPage();
                break;

            case 1:
            default:
                MainMenuUI::main();
        }
    }
    
    system("pause"); // waits for a keypress

    
    return 0;
}

/*
    Menu menu;
    Order order;
    Order::Item item;
    unordered_map<int, Order::Item> menuList;

    cout<<"---------- Menu ----------"<<endl;
    for (auto& i:menu.menuList(false, true)){
        cout<<i.at("id") + " | " + i.at("name") + " | RM " + i.at("selling_price") + " | " + i.at("category") << endl;

        item.id = stoi(i.at("id"));
        item.name = i.at("name");
        item.productionCost = stod(i.at("production_cost"));
        item.sellingPrice = stod(i.at("selling_price"));
        item.quantity = 0;

        menuList[stoi(i.at("id"))] = item;
    }
    cout<<"\n"<<endl;

    string input;
    vector<Order::Item> cart;

    while (true){
        cout<<"---------- Cart ----------"<<endl;
        cart = order.itemsList();

        if (cart.empty()){
            cout<< "Cart is empty!" << endl;
        }
        else{
            for (auto& i:cart){
                cout << i.id << " | " << i.name << " | RM " << fixed << setprecision(2) << i.sellingPrice << " | " << i.quantity << endl;
            }
        }

        cout<<"\n\nChoose menu or close (X): ";
        cin>>input;

        if (input == "X" || input == "x"){
            break;
        }
        else if (input == "d" || input == "D"){
            cout<<"\n\nDelete: ";
            cin>>input;
            auto it = menuList.find(stoi(input));

            if (it != menuList.end()) {
                order.removeItems(stoi(input));
            }
            else{
                cout<<"Invalid Menu"<<endl;
            }
        }
        else if (input == "="){
            cout<<"Submit Order"<<endl;
            order.createOrder();
            break;
        }
        else{
            auto it = menuList.find(stoi(input));

            if (it != menuList.end()) {
                item = menuList[stoi(input)];
                order.addItem(item.id, item.name, item.productionCost, item.sellingPrice, 1);
            }
            else{
                cout<<"Invalid Menu"<<endl;
            }
        }
    }
    */
