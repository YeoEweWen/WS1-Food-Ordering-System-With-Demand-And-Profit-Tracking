#include "core/menu.h"
#include "core/auth.h"
#include "core/database.h"
#include "utils/functions.h"

using namespace std;

// Private Methods
bool Menu::isCategoryDeletable(int id){
    Database db;

    string query = "SELECT category_id FROM menu WHERE category_id = :id LIMIT 1;";
    map<string, string> params = {
        {"id", to_string(id)}
    };

    vector<map<string, string>> result = db.fetchData(query, params);

    return result.empty();
}

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
bool Menu::addCategory(string categoryName, string type){
    Database db;
    Auth auth;

    Auth::UserDetails userDetails = auth.retrieveLoggedUserDetails();

    string query = "INSERT INTO menu_category (category, type, created_by) VALUES (:category, :type, :created_by);";
    map<string, string> params = {
        {"category", categoryName},
        {"type", type},
        {"created_by", to_string(userDetails.id)}
    };

    return db.runQuery(query, params);
}

bool Menu::updateCategoryName(int id, string name){
    Database db;
    Auth auth;

    Auth::UserDetails userDetails = auth.retrieveLoggedUserDetails();

    string query = "UPDATE menu_category SET category = :category, updated_by = :updated_by, updated_at = :updated_at WHERE id = :id;";
    map<string, string> params = {
        {"id", to_string(id)},
        {"category", name},
        {"updated_by", to_string(userDetails.id)},
        {"updated_at", timestamp()}
    };

    return db.runQuery(query, params);
}

bool Menu::updateCategoryType(int id, string type){
    Database db;
    Auth auth;

    Auth::UserDetails userDetails = auth.retrieveLoggedUserDetails();

    string query = "UPDATE menu_category SET type = :type, updated_by = :updated_by, updated_at = :updated_at WHERE id = :id;";
    map<string, string> params = {
        {"id", to_string(id)},
        {"type", ((toLowerCase(type) == "food") ? "Food" : "Beverage")},
        {"updated_by", to_string(userDetails.id)},
        {"updated_at", timestamp()}
    };

    return db.runQuery(query, params);
}

bool Menu::deleteCategory(int id){
    Database db;

    if (!isCategoryDeletable(id)){
        logInfo("This menu category (ID: " + to_string(id) + ") cannot be deleted.");
        return false;
    }

    string query = "DELETE FROM menu_category WHERE id = :id;";
    map<string, string> params = {
        {"id", to_string(id)}
    };

    return db.runQuery(query, params);
}

vector<map<string, string>> Menu::categoryList(){
    Database db;

    string query = "SELECT id, category, type FROM menu_category ORDER BY category ASC;";

    return db.fetchData(query);
}

Menu::CategoryDetails Menu::categoryDetails(int id){
    Database db;

    string query = "SELECT mc.id, mc.category, mc.type, mc.created_at, mc.created_by AS created_by_id, u.name AS created_by_name, "
                   "mc.updated_at, mc.updated_by AS updated_by_id, u2.name AS updated_by_name FROM menu_category mc "
                   "LEFT JOIN user AS u ON u.id = mc.created_by "
                   "LEFT JOIN user AS u2 ON u2.id = mc.updated_by "
                   "WHERE mc.id = :id;";

    map<string, string> params = {{"id", to_string(id)}};

    vector<map<string, string>> result = db.fetchData(query, params);

    if (result.empty()){
        return {-1};
    }

    map<string, string> details = result[0];

    return {
        stoi(details.at("id")),
        details.at("category"),
        details.at("type"),
        details.at("created_at"),
        stoi(details.at("created_by_id")),
        details.at("created_by_name"),
        ((details.at("updated_at") == "NULL") ? "-" : details.at("updated_at")),
        ((details.at("updated_by_id") == "NULL") ? -1 : stoi(details.at("updated_by_id"))),
        ((details.at("updated_by_name") == "NULL") ? "-" : details.at("updated_by_name")),
        isCategoryDeletable(id)
    };
}


// Menu
bool Menu::addMenu(string name, double productionCost, double sellingPrice, int categoryID){
    Database db;
    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();

    string query = "INSERT INTO menu (name, production_cost, selling_price, category_id, created_by) VALUES (:name, :production_cost, :selling_price, :category_id, :created_by);";
    map<string, string> params = {
        {"name", name},
        {"production_cost", formatDecimalPoints(productionCost, 2)},
        {"selling_price", formatDecimalPoints(sellingPrice, 2)},
        {"category_id", to_string(categoryID)},
        {"created_by", to_string(userDetails.id)},
    };

    return db.runQuery(query, params);
}

bool Menu::updateMenuName(int id, std::string name){
    Database db;
    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();

    string query = "UPDATE menu SET name = :name, updated_by = :updated_by, updated_at = :updated_at "
                   "WHERE id = :id AND availability = 'Available';";
    map<string, string> params = {
        {"id", to_string(id)},
        {"name", name},
        {"updated_by", to_string(userDetails.id)},
        {"updated_at", timestamp()},
    };

    return db.runQuery(query, params);
}

bool Menu::updateMenuCostAndPrice(int id, double productionCost, double sellingPrice){
    Database db;
    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();

    string query = "UPDATE menu SET production_cost = :production_cost, selling_price = :selling_price, updated_by = :updated_by, updated_at = :updated_at "
                   "WHERE id = :id AND availability = 'Available';";
    map<string, string> params = {
        {"id", to_string(id)},
        {"production_cost", formatDecimalPoints(productionCost, 2)},
        {"selling_price", formatDecimalPoints(sellingPrice, 2)},
        {"updated_by", to_string(userDetails.id)},
        {"updated_at", timestamp()},
    };

    return db.runQuery(query, params);
}

bool Menu::updateMenuCategory(int id, int categoryID){
    Database db;
    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();

    string query = "UPDATE menu SET category_id = :category_id, updated_by = :updated_by, updated_at = :updated_at "
                   "WHERE id = :id AND availability = 'Available';";
    map<string, string> params = {
        {"id", to_string(id)},
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

TableList Menu::menuList(string search, string sortColumn, bool sortAsc, int page, int limitRowPerPage){
    Database db;

    vector<string> sortableColumnKeys = {"name", "price", "category", "availability"};

    string orderQuery = "ORDER BY m.created_at DESC, m.updated_at DESC ";
    if (find(sortableColumnKeys.begin(), sortableColumnKeys.end(), sortColumn) != sortableColumnKeys.end()){
        if (sortColumn == "price"){
            sortColumn = "m.selling_price";
        }
        else if (sortColumn == "category"){
            sortColumn = "mc.category";
        }
        else{
            sortColumn = "m." + sortColumn;
        }

        orderQuery = "ORDER BY " + sortColumn + " " + ((sortAsc) ? "ASC " : "DESC ");
    }
    
    int offset = limitRowPerPage * (page - 1);
    string limitOffsetQuery = "LIMIT " + to_string(limitRowPerPage) + " OFFSET " + to_string(offset) + ";";

    // Retrieve the total of row (Without limit)
    string query = "SELECT COUNT(*) AS total FROM menu m "
                   "LEFT JOIN menu_category AS mc ON mc.id = m.category_id "
                   "WHERE (m.name LIKE :search OR m.selling_price LIKE :search OR mc.category LIKE :search OR m.availability LIKE :search);";

    map<string, string> params = {
        {"search", "%" + search + "%"}
    };

    int total = stoi(db.fetchData(query, params)[0].at("total"));

    // Retrieve the rows (With limit)
    query = "SELECT m.id, m.name, m.selling_price AS price, mc.category AS category, m.availability FROM menu m "
            "LEFT JOIN menu_category AS mc ON mc.id = m.category_id "
            "WHERE (m.name LIKE :search OR m.selling_price LIKE :search OR mc.category LIKE :search OR m.availability LIKE :search) "
             + orderQuery + limitOffsetQuery;

    vector<map<string, string>> list = db.fetchData(query, params);

    return {list, total};
}

Menu::MenuDetails Menu::menuDetails(int id){
    Database db;

    string query = "SELECT m.id, m.name, m.production_cost, m.selling_price, mc.category AS category, mc.type, "
                   "m.availability, m.created_at, m.created_by AS created_by_id, u.name AS created_by_name, "
                   "m.updated_at, m.updated_by AS updated_by_id, u2.name AS updated_by_name FROM menu m "
                   "LEFT JOIN menu_category AS mc ON mc.id = m.category_id "
                   "LEFT JOIN user AS u ON u.id = m.created_by "
                   "LEFT JOIN user AS u2 ON u2.id = m.updated_by "
                   "WHERE m.id = :id;";

    map<string, string> params = {{"id", to_string(id)}};

    vector<map<string, string>> result = db.fetchData(query, params);

    if (result.empty()){
        return {-1};
    }

    map<string, string> details = result[0];

    return {
        stoi(details.at("id")),
        details.at("name"),
        stod(details.at("production_cost")),
        stod(details.at("selling_price")),
        calculateProfitMargin(stod(details.at("production_cost")), stod(details.at("selling_price"))),
        details.at("category"),
        details.at("type"),
        details.at("availability"),
        details.at("created_at"),
        stoi(details.at("created_by_id")),
        details.at("created_by_name"),
        ((details.at("updated_at") == "NULL") ? "-" : details.at("updated_at")),
        ((details.at("updated_by_id") == "NULL") ? -1 : stoi(details.at("updated_by_id"))),
        ((details.at("updated_by_name") == "NULL") ? "-" : details.at("updated_by_name")),
        isMenuDeletable(id),
    };
}

vector<map<string, string>> Menu::menuPriceList(){
    Database db;

    string query = "SELECT m.id, m.name, m.production_cost, m.selling_price, mc.category AS category, mc.type FROM menu m "
                   "LEFT JOIN menu_category AS mc ON mc.id = m.category_id "
                   "WHERE m.availability = 'Available' ORDER BY m.name ASC, mc.category ASC;";

    return db.fetchData(query);
}