#include "users.h"
#include "auth.h"
#include "database.h"
#include "functions.h"

using namespace std;

/*---------- ADMIN ONLY ----------*/
bool Users::registerUser(string name, string username, string role, string password){
    
}

bool Users::updateUsername(int id, string newUsername){

}

bool Users::updateRole(int id, string newRole){

}

bool Users::deactivate(int id){

}

bool Users::activate(int id){

}

bool Users::resetPassword(int id, string newPassword){

}

vector<map<string, string>> Users::userList(){

}

/*---------- ALL USERS ----------*/
