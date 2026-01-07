// Core
#include "core/auth.h"
#include "core/analytics.h"

// UIManager
#include "ui/auth.h"
#include "ui/main_menu.h"
#include "ui/menu.h"
#include "ui/order.h"
#include "ui/profile.h"
#include "ui/reports.h"
#include "ui/store.h"
#include "ui/transaction_history.h"
#include "ui/users.h"
#include "ui/ui_manager.h"

// Utils
#include "utils/functions.h"

#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    Page page;

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
                MenuUI::main();
                break;

            case 4:
                OrderUI::main();
                break;

            case 5:
                TransactionHistoryUI::main();
                break;

            case 6:
                ReportUI::main();
                break;

            case 7:
                StoreUI::main();
                break;

            case 8:
                ProfileUI::main();
                break;

            case 1:
            default:
                MainMenuUI::main();
        }
    }
    
    return 0;
}


/*

// Data Seeding
#include "core/database.h"
#include "core/menu.h"
#include "core/order.h"
#include <vector>
#include <map>
#include <random>
#include <algorithm>

using namespace std;


int endDay = 7;
int month = 1;
int year = 2026;
int maxOperationTime = 600; // 10 Hours (7am - 5pm)
int currentMinute = 0; // In minutes

int randomUserID(){
    vector<int> userID = {2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6};
    mt19937 gen(random_device{}());
    shuffle(userID.begin(), userID.end(), gen);

    return userID[0];
}

string runningDatetime(int day, vector<int> skipPeriods){
    mt19937 gen(random_device{}());
    shuffle(skipPeriods.begin(), skipPeriods.end(), gen);

    int skip = skipPeriods[0];

    currentMinute += skip;

    int hour = 7 + currentMinute/60;
    int minute = currentMinute%60;

    string date = to_string(year) + ((month < 10) ? "-0" : "-") + to_string(month) + ((day < 10) ? "-0" : "-") + to_string(day);
    string time = ((hour < 10) ? "0" : "") + to_string(hour) + ((minute < 10) ? ":0" : ":") + to_string(minute) + ":00";

    return  date + " " + time;
}

void transactionSeeding(){
    cout<<"Start Seeding..."<<endl;

    Menu menu;
    Order order;

    vector<map<string, string>> menuList = menu.menuPriceList();

    int i, j;

    vector<vector<int>> skipPeriodPaterns = {
        {11, 12, 3, 40},
        {10, 22, 50, 10},
        {51, 14, 15, 14},
        {30, 14, 5, 60},
        {14, 6, 18, 15}
    };

    for (i=1; i<=endDay; i++){
        // Month
        currentMinute = 0;

        mt19937 gen(random_device{}());
        shuffle(skipPeriodPaterns.begin(), skipPeriodPaterns.end(), gen);

        // Day
        while (currentMinute <= maxOperationTime - 10){
            order.clearItems();

            uniform_int_distribution<int> dist(1, 5);
            int totalItems = dist(gen);

            for (j=0; j<totalItems; j++){
                uniform_int_distribution<int> dist2(1, 3);
                int quantity = dist2(gen);

                uniform_int_distribution<int> dist3(0, menuList.size()-1);
                int selectedIndex = dist3(gen);

                order.addItem(
                    stoi(menuList[selectedIndex].at("id")), 
                    menuList[selectedIndex].at("name"),
                    stod(menuList[selectedIndex].at("production_cost")),
                    stod(menuList[selectedIndex].at("selling_price")),
                    quantity
                );
            }

            order.createOrder(randomUserID(), runningDatetime(i, skipPeriodPaterns[0]));
        }

        cout<<"Day "<<i<<" Completed..."<<endl;
    }

    cout<<"Task Completed!"<<endl;
    string temp;
    cin>>temp;
}

*/