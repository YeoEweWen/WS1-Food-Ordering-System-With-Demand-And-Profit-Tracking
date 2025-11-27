#include "menu.h"
#include "auth.h"
#include "database.h"
#include "functions.h"

using namespace std;

// Private Methods
bool Menu::isMenuDeletable(int id){
    Database db;

    string query = "SELECT menu_id FROM order_items WHERE menu_id = :id LIMIT 1;";
    map<string, string> params = {
        {"id", to_string(id)}
    };

    vector<map<string, string>> result = db.fetchData(query, params);

    return result.empty();
}

// Public Methods
// Menu Category
bool Menu::addMenuCategory(string categoryName){
    Database db;

    string query = "INSERT INTO menu_category (category) VALUES (:category);";
    map<string, string> params = {
        {"category", categoryName}
    };

    return db.runQuery(query, params);
}

bool Menu::updateMenuCategory(int id, string categoryName){
    Database db;

    string query = "UPDATE menu_category SET category = :category WHERE id = :id;";
    map<string, string> params = {
        {"id", to_string(id)},
        {"category", categoryName}
    };

    return db.runQuery(query, params);
}

vector<map<string, string>> Menu::categoryList(){
    Database db;

    string query = "SELECT * FROM menu_category;";

    return db.fetchData(query);
}

// Menu
bool Menu::addMenu(string name, double productionCost, double sellingPrice, int categoryID){
    Database db;
    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();

    string query = "INSERT INTO menu (name, production_cost, selling_price, category_id, created_by) VALUES (:name, :production_cost, :selling_price, :category_id, :created_by);";
    map<string, string> params = {
        {"name", name},
        {"production_cost", to_string(productionCost)},
        {"selling_price", to_string(sellingPrice)},
        {"category_id", to_string(categoryID)},
        {"created_by", to_string(userDetails.id)},
    };

    return db.runQuery(query, params);
}

bool Menu::updateMenu(int id, string name, double productionCost, double sellingPrice, int categoryID){
    Database db;
    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();

    string query = "UPDATE menu SET "
                   "    name = :name, production_cost = :production_cost, selling_price = :selling_price, category_id = :category_id, updated_by = :updated_by, updated_at = :updated_at "
                   "WHERE id = :id AND availablity = 'Available';";
    map<string, string> params = {
        {"id", to_string(id)},
        {"name", name},
        {"production_cost", to_string(productionCost)},
        {"selling_price", to_string(sellingPrice)},
        {"category_id", to_string(categoryID)},
        {"updated_by", to_string(userDetails.id)},
        {"updated_at", timestamp()},
    };

    return db.runQuery(query, params);
}

bool Menu::deleteMenu(int id){
    Database db;

    if (!isMenuDeletable(id)){
        logInfo("This menu (ID: " + to_string(id) + ") cannot be deleted.");
        return false;
    }

    string query = "DELETE FROM menu WHERE id = :id;";
    map<string, string> params = {
        {"id", to_string(id)}
    };

    return db.runQuery(query, params);
}

double Menu::calculateProfitMargin(double productionCost, double sellingPrice){
    return ((sellingPrice - productionCost) / sellingPrice) * 100;
}

bool Menu::setAvailable(int id){
    Database db;

    string query = "UPDATE menu SET availability = 'Available' WHERE id = :id;";
    map<string, string> params = {
        {"id", to_string(id)}
    };

    return db.runQuery(query, params);
}

bool Menu::setUnavailable(int id){
    Database db;

    string query = "UPDATE menu SET availability = 'Unavailable' WHERE id = :id;";
    map<string, string> params = {
        {"id", to_string(id)}
    };

    return db.runQuery(query, params);
}

vector<map<string, string>> Menu::menuList(bool availableOnly){
    Database db;

    string availabilityCondition = "";
    if (availableOnly){
        availabilityCondition = "WHERE m.availability = 'Available' ";
    }

    string query = "SELECT "
                   "    m.id, m.name, m.production_cost, m.selling_price, m.category_id, mc.category AS category, m.availability, "
                   "    m.created_by AS created_by_id, u.name AS created_by, m.created_at, "
                   "    m.updated_by AS updated_by_id, u2.name AS updated_by, m.updated_at "
                   "FROM menu m "
                   "LEFT JOIN menu_category AS mc ON mc.id = m.category "
                   "LEFT JOIN user AS u ON u.id = m.created_by "
                   "LEFT JOIN user AS u2 ON u2.id = m.updated_by "
                   + availabilityCondition + 
                   "ORDER BY mc.category ASC, m.name ASC;";

    return db.fetchData(query);
}
