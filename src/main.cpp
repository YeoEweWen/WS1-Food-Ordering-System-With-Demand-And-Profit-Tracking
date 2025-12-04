#include "auth.h"
#include "menu.h"
#include "users.h"
#include "functions.h"

#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

int main() {
    cout << "##### Food Ordering System with Demand and Profit Tracking #####\n" << endl;

    Menu menu;

    printVectorValues(menu.menuList(true));


    system("pause"); // waits for a keypress

    return 0;
}
