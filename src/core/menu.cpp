#include "core/menu.h"
#include "core/auth.h"
#include "core/database.h"
#include "utils/functions.h"

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
    Auth auth;

    Auth::UserDetails userDetails = auth.retrieveLoggedUserDetails();

    string query = "INSERT INTO menu_category (category, created_by) VALUES (:category, :created_by);";
    map<string, string> params = {
        {"category", categoryName},
        {"created_by", to_string(userDetails.id)}
    };

    return db.runQuery(query, params);
}

bool Menu::updateMenuCategory(int id, string categoryName, string type){
    Database db;
    Auth auth;

    Auth::UserDetails userDetails = auth.retrieveLoggedUserDetails();

    string query = "UPDATE menu_category SET category = :category, type = :type, updated_by = :updated_by, updated_at = :updated_at WHERE id = :id;";
    map<string, string> params = {
        {"id", to_string(id)},
        {"category", categoryName},
        {"type", ((toLowerCase(type) == "food") ? "Food" : "Beverage")},
        {"updated_by", to_string(userDetails.id)},
        {"updated_at", timestamp()}
    };

    return db.runQuery(query, params);
}

vector<map<string, string>> Menu::categoryList(bool detailedList){
    Database db;

    string query;

    if (detailedList){
        query = "SELECT "
                "   mc.id, mc.category, "
                "   mc.created_by AS created_by_id, u.name AS created_by, mc.created_at, "
                "   mc.updated_by AS updated_by_id, u2.name AS updated_by, mc.updated_at "
                "FROM menu_category mc "
                "LEFT JOIN user AS u ON u.id = mc.created_by "
                "LEFT JOIN user AS u2 ON u2.id = mc.updated_by "
                "ORDER BY mc.updated_at DESC, mc.created_at DESC;";
    }
    else{
        query = "SELECT id, category FROM menu_category ORDER BY category ASC;";
    }

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
                   "WHERE id = :id AND availability = 'Available';";
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

vector<Menu::MenuDetails> Menu::menuPriceList(){
    Database db;

    string query = "SELECT m.id, m.name, m.selling_price, mc.name AS category FROM menu m "
                   "LEFT JOIN menu_category AS mc ON mc.id = m.category_id "
                   "WHERE m.availability = 'Available' "
                   "ORDER BY m.name ASC";
}