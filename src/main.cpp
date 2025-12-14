#include "core/auth.h"
#include "core/menu.h"
#include "core/order.h"
#include "core/users.h"
#include "utils/functions.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <iomanip>

using namespace std;

int main() {
    cout << "##### Food Ordering System with Demand and Profit Tracking #####\n" << endl;

    Order order;

    Order::OrderDetails details = order.orderDetails(2);

    cout<<details.createdBy<<endl;
    cout<<details.createdAt<<endl;

    for (auto i:details.itemsList){
        cout<<i.name<<endl;
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

    system("pause"); // waits for a keypress

    return 0;
}
