/*

Course: BAXU 2931 Workshop I
Project Title: Food Ordering System with Demand and Profit Tracking
Developed By: Yeo Ewe Wen B152510006

*/

// Core
#include "core/auth.h"

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