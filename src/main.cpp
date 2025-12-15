// Core
#include "core/auth.h"
#include "core/menu.h"
#include "core/order.h"
#include "core/users.h"

// UI
#include "ui/loginUI.h"
#include "ui/usersUI.h"

// Utils
#include "utils/functions.h"


#include <iostream>

using namespace std;

void clearConsole (){
    system("cls");
}

int main() {
    while (true){
        system("cls");
        if (!Auth::isLoggedIn()){
            LoginUI::main();
            continue;
        }

        cout<<"This is main page"<<endl;
        system("pause"); // waits for a keypress
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
