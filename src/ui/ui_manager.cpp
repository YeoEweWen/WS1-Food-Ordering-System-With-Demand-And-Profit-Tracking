#include "ui/ui_manager.h"
#include "core/auth.h"
#include "utils/functions.h"

using namespace std;

Page UIManager::currentPage = {0, 0, {}, {}};
int UIManager::lineLength = 120;

// ---------- Pages & Navigation ----------
Page UIManager::currentPageDetails(){
    return currentPage;
}

void UIManager::goTo(int id, int subID){
    currentPage.id = id;
    currentPage.subID = subID;
}

void UIManager::clearParams(){
    currentPage.params = {};
}

void UIManager::clearErrorMessages(){
    currentPage.errorMessages = {};
}

void UIManager::addParam(string key, string value){
    currentPage.params[key] = value;
}

void UIManager::addErrorMessage(string errorMessage){
    currentPage.errorMessages.push_back(errorMessage);
}


// ---------- Components ----------
void UIManager::header(const string& pageName){
    system("cls");

    int space;
    string storeName = "Ahmad Cafe";

    cout<<string(lineLength, '-')<<endl<<endl;
    space = (lineLength-11-countStringLength(storeName))/2;
    cout<<string(space, ' ')<<"Welcome to "<<storeName<<endl<<endl;
    cout<<string(lineLength, '-')<<endl;

    string firstName = "";

    if (Auth::isLoggedIn()){
        string name = Auth::retrieveLoggedUserDetails().name;
        stringstream ss(name);
        ss >> firstName;
        firstName = "Hi, " + firstName;
    }

    space = lineLength - countStringLength(pageName) - countStringLength(firstName);
    cout<<toUpperCase(pageName)<<string(space, ' ')<<firstName<<endl;
    cout<<string(lineLength, '-')<<endl;
}

void UIManager::errorMessages(const vector<string>& errorMessages, bool topBorder, bool bottomBorder){
    if (errorMessages.empty()){
        return;
    }

    if (topBorder){
        cout<<string(lineLength, '-')<<endl;
    }

    if (errorMessages.size() > 1){
        // Multi Line
        int i = 1;
        cout<<"Errors:"<<endl;
        for (const auto& msg : errorMessages){
            cout<<i<<". "<<msg<<endl;
            i++;
        }
    }
    else{
        // Single Line
        cout<<"Error: "<<errorMessages[0]<<endl;
    }
    
    if (bottomBorder){
        cout<<string(lineLength, '-')<<endl;
    }
}

void UIManager::emptyPage(bool includeHeader){
    if (includeHeader){
        header("empty page");
    }
    
    int space = (lineLength-19)/2;
    cout<<endl<<string(space, ' ')<<"This is empty page."<<endl<<endl;
    cout<<string(lineLength, '-')<<endl;

    system("pause"); // waits for a keypress
    exit(0);
}