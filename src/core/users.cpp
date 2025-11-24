#include "users.h"
#include "auth.h"
#include "database.h"
#include "functions.h"

using namespace std;


string Users::generateUsername(string name){
    return "";
}

/*---------- ADMIN ONLY ----------*/
bool Users::registerUser(string name, string role, string password){ // Add password hashing
    Database db;

    string username = name + role;
    string query = "INSERT INTO user (name, username, role, password, created_by) VALUES (:name, :username, :role, :password, :created_by);";
    map<string, string> params = {
        {"name", name},
        {"username", username},
        {"role", role},
        {"password", password},
        {"created_by", "NULL"}, // Replace with current logged user id
    };

    return db.runQuery(query, params);
}

bool Users::updateRole(int id, string newRole){
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

bool Users::resetPassword(int id, string newPassword){ // Add password hashing
    Database db;

    string query = "UPDATE user SET password = :password WHERE id = :id AND status = 'Active';";
    map<string, string> params = {
        {"id", to_string(id)},
        {"password", newPassword}
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
