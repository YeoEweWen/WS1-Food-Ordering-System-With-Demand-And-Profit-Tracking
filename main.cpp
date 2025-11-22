#include "database.h"
#include "functions.h"
#include <iostream>

using namespace std;

int main() {
    Database db;

    map<string, string> params;

    // Usage Examples
    // CREATE
    params = {
        {"name", "Mohd Faizal Bin Mohd Safuan"},
        {"email", "faizal@email.com"},
        {"department_id", "3"},
    };
    db.runQuery("INSERT INTO employee (name, email, department_id) VALUES (:name, :email, :department_id);", params); // Return bool, Success = true, Fail = false.

    // READ
    params = {
        {"department_id", "1"},
    };
    vector<map<string, string>> retrievedRows = db.fetchData("SELECT * FROM employee WHERE department_id = :department_id;", params);
    printVectorValues(retrievedRows);

    // UPDATE
    params = {
        {"id", "1"},
        {"name", "Mohd Faizal Shamil Bin Mohd Safuan"},
    };
    db.runQuery("UPDATE employee SET name = :name WHERE id = :id;", params); // Return bool, Success = true, Fail = false.

    // DELETE
    params = {
        {"id", "5"},
    };
    db.runQuery("DELETE FROM employee WHERE id = :id;", params); // Return bool, Success = true, Fail = false.


    cout << "Press Enter to exit...";
    cin.get();

    return 0;
}
