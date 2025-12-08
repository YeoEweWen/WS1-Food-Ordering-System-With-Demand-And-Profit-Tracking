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
    printVectorValues(menu.menuList(false, true));


    system("pause"); // waits for a keypress

    return 0;
}
