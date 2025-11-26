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

    Auth::LoginStatus loginStatus;

    string username, password;

    cout << "\n\nUsername: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    loginStatus = Auth::login(username, password);

    cout << "\n\nSucess: " << (loginStatus.success ? "Yes" : "No") << endl;
    cout << "Error Code: " << loginStatus.errorCode << endl; 
    cout << "Description: " << loginStatus.description << endl;
    cout << "Preset Password: " << (loginStatus.usingPresetPassword ? "Yes" : "No") << endl;


    system("pause"); // waits for a keypress

    return 0;
}
