//#include "database.h"
#include "auth.h"
#include "users.h"
#include "functions.h"
#include <iostream>

using namespace std;

int main() {
    cout << "##### Food Ordering System with Demand and Profit Tracking #####\n\n" << endl;

    Users users;

    bool result = users.registerUser("Abu", "Employee", "pass1234");
    cout << result << endl;

    printVectorValues(users.userList());


    cout << "Press Enter to exit...";
    cin.get();

    return 0;
}
