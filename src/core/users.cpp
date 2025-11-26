#include "users.h"
#include "auth.h"
#include "database.h"
#include "functions.h"

using namespace std;


string Users::generateUsername(string name){
    Database db;

    string firstName;
    vector<int> usedUniqueNumbers, availableUniqueNumbers;
    int uniqueNumber;
    string paddedUniqueNumber; // Fill 0 at the front to make it 4 digits

    // Extract the first name and convert it into lower case
    stringstream ss(name);
    ss >> firstName;
    transform(firstName.begin(), firstName.end(), firstName.begin(), ::tolower);

    // Retrieve all the username with same initial first name, and store all the used unique number
    string query = "SELECT username FROM user WHERE username LIKE :username;";
    map<string, string> params = {
        {"username", firstName + "%"}
    };

    vector<map<string, string>> usernames = db.fetchData(query, params);

    for (const auto& row : usernames) {
        uniqueNumber = stoi(row.at("username").substr(firstName.length()));
        usedUniqueNumbers.push_back(uniqueNumber);
    }

    // Generate all available options for the unique number
    for (int i=0; i<10000; i++){
        if (std::find(usedUniqueNumbers.begin(), usedUniqueNumbers.end(), i) != usedUniqueNumbers.end()) {
            continue; // skip this number
        }
        
        availableUniqueNumbers.push_back(i);
    }

    // Pick one unique number
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, availableUniqueNumbers.size() - 1); // Distribution from 0 to vector.size() - 1
    uniqueNumber = availableUniqueNumbers[dis(gen)];

    // Fill the 0 at the front to make it 4 digits.
    paddedUniqueNumber = to_string(uniqueNumber);
    paddedUniqueNumber = string(4 - paddedUniqueNumber.length(), '0') + paddedUniqueNumber;

    return firstName + paddedUniqueNumber;
}

bool Users::isValidRole(string role){
    transform(role.begin(), role.end(), role.begin(), ::tolower); // convert to lowercase
    return ((role == "admin") || (role == "employee"));
}

/*---------- ADMIN ONLY ----------*/
map<string, string> Users::registerUser(string name, string role){
    Database db;

    string username = generateUsername(name);
    string salt = Auth::generateSalt();
    string password = Auth::hashPassword(username, salt);

    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();

    string query = "INSERT INTO user (name, username, role, password, salt, created_by) VALUES (:name, :username, :role, :password, :salt, :created_by);";
    map<string, string> params = {
        {"name", name},
        {"username", username},
        {"role", role},
        {"password", password},
        {"salt", salt},
        {"created_by", to_string(userDetails.id)},
    };

    if (!db.runQuery(query, params)){
        logError("Failed to register new user.");
        return {
            {"status", "fail"}
        };
    }

    return {
        {"name", name},
        {"username", username},
        {"role", role},
        {"password", password},
        {"status", "success"}
    };
}

bool Users::updateRole(int id, string newRole){
    if (!isValidRole(newRole)){
        logError(newRole + " is not a valid role.");
        return false;
    }

    newRole = toProperCase(newRole);

    Database db;

    string query = "UPDATE user SET role = :role WHERE id = :id AND status = 'Active';";
    map<string, string> params = {
        {"id", to_string(id)},
        {"role", newRole}
    };

    return db.runQuery(query, params);
}

bool Users::deactivate(int id){
    Database db;

    string query = "UPDATE user SET status = 'Inactive' WHERE id = :id AND status = 'Active';";
    map<string, string> params = {
        {"id", to_string(id)}
    };

    return db.runQuery(query, params);
}

bool Users::activate(int id){
    Database db;

    string query = "UPDATE user SET status = 'Active' WHERE id = :id AND status = 'Inactive';";
    map<string, string> params = {
        {"id", to_string(id)}
    };

    return db.runQuery(query, params);
}

bool Users::resetPassword(int id){
    Database db;
    Auth auth;

    string query, username, status, newPassword, newSalt;
    map<string, string> params;
    vector<map<string, string>> result;

    // Retrieve the username & status
    query = "SELECT username, status FROM user WHERE id = :id LIMIT 1;";
    params = {
        {"id", to_string(id)}
    };
    result = db.fetchData(query, params);

    if (result.empty()){
        logError("User (ID: " + to_string(id) + ") not found.");
        return false;
    }

    username = result[0].at("username");
    status = result[0].at("status");

    if (status == "Inactive"){
        logError("User (ID: " + to_string(id) + ") is inactive.");
        return false;
    }

    newSalt = Auth::generateSalt();
    newPassword = Auth::hashPassword(username, newSalt);

    query = "UPDATE user SET password = :password, salt = :salt WHERE id = :id AND status = 'Active';";
    params = {
        {"id", to_string(id)},
        {"password", newPassword},
        {"salt", newSalt}
    };

    return db.runQuery(query, params);
}

vector<map<string, string>> Users::userList(){
    Database db;

    string query = "SELECT u.id, u.name, u.username, u.role, u.created_by AS created_by_id, u2.name AS created_by, u.created_at, u.last_logged_in, u.status "
                   "FROM user u "
                   "LEFT JOIN user AS u2 ON u2.id = u.created_by "
                   "ORDER BY u.status ASC, u.last_logged_in DESC;";

    return db.fetchData(query);
}

/*---------- ALL USERS ----------*/
bool Users::updateName(string newName){
    Database db;
    Auth auth;

    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();

    string query = "UPDATE user SET name = :name WHERE id = :id AND status = 'Active';";
    map<string, string> params = {
        {"id", to_string(userDetails.id)},
        {"name", newName}
    };

    return db.runQuery(query, params);
}

bool Users::updatePassword(string newPassword){
    Database db;
    Auth auth;

    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();
    string password, salt, query;

    salt = Auth::generateSalt();
    password = Auth::hashPassword(newPassword, salt);

    query = "UPDATE user SET password = :password, salt = :salt WHERE id = :id AND status = 'Active';";
    map<string, string> params = {
        {"id", to_string(userDetails.id)},
        {"password", password},
        {"salt", salt}
    };

    return db.runQuery(query, params);
}