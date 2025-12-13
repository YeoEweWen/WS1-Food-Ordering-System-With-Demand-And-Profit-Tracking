#include "core/auth.h"
#include "core/menu.h"
#include "core/order.h"
#include "core/users.h"
#include "utils/functions.h"

#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

int main() {
    cout << "##### Food Ordering System with Demand and Profit Tracking #####\n" << endl;

    Menu menu;
    Order order;

    // ID, Name, Price, Category
    vector<map<string, string>> menuList = menu.menuList(false, true);

    cout<<"---------- Menu ----------"<<endl;
    for (auto& item:menuList){
        cout<<item.at("id") + " | " + item.at("name") + " | RM " + item.at("selling_price") + " | " + item.at("category") << endl;
    }
    cout<<"\n"<<endl;

    string input = "";
    while (input != "X" && input != "x"){
        cout<<"Choose menu or close (X): ";
        cin>>input;
    }

    system("pause"); // waits for a keypress

    return 0;
}
