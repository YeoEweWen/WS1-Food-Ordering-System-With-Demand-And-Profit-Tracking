#include "core/order.h"
#include "core/auth.h"
#include "core/database.h"
#include "utils/functions.h"

using namespace std;

Order::Order() {
    clearItems();
}

// Items
void Order::clearItems() {
    items = {};
}

void Order::addItem(int id, string name, double productionCost, double sellingPrice, int quantity){
    Order::Item item;
    
    if (!items.empty()){
        // Check for id existing. Increase the quantity instead of add new if already exist.
        auto it = std::find_if(items.begin(), items.end(), [id](const Item& item){ return item.id == id; });

        if (it != items.end()) {
            int newQuantity = it->quantity + quantity;
            updateQuantity(id, newQuantity);
            return;
        }
    }

    item.id = id;
    item.name = name;
    item.productionCost = productionCost;
    item.sellingPrice = sellingPrice;
    item.quantity = quantity;

    items.push_back(item);
}

void Order::updateQuantity(int id, int newQuantity){
    auto it = std::find_if(items.begin(), items.end(), [id](const Item& item){ return item.id == id; });

    if (it != items.end()) {
        it->quantity = newQuantity;
    } 
    else {
        logError("Failed to update item's quantity. Item (ID: )" + to_string(id) + ") not found.");
    }
}

void Order::removeItems(int id) {
    auto it = std::remove_if(items.begin(), items.end(), [id](const Item& item) {
            return item.id == id;  
        }
    );

    if (it != items.end()) {
        items.erase(it, items.end()); // actually remove from vector
    }
    else{
        logError("Failed to remove item. Item (ID: )" + to_string(id) + ") not found.");
    }
}

const vector<Order::Item>& Order::itemsList() const {
    return items;
}

// Order
bool Order::createOrder() {
    Database db;
    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();

    string query;
    map<string, string> params;

    if (items.empty()){
        logError("Failed to create order. The items is empty.");
        return false;
    }

    // Create new order
    query = "INSERT INTO `order` (created_by) VALUES (:created_by);";
    params = {
        {"created_by", to_string(userDetails.id)}
    };

    if (!db.runQuery(query, params)){
        logError("Failed to create order.");
        return false;
    }

    // Retrieve the order id
    query = "SELECT id FROM `order` WHERE created_by = :created_by AND transaction_status = 'Completed' ORDER BY created_at DESC LIMIT 1;";
    string orderID = db.fetchData(query, params)[0].at("id");

    // Insert the items
    for (const auto& item_ : items){
        query = "INSERT INTO order_items (order_id, menu_id, name, production_cost, selling_price, quantity) VALUES (:order_id, :menu_id, :name, :production_cost, :selling_price, :quantity);";
        params = {
            {"order_id", orderID},
            {"menu_id", to_string(item_.id)},
            {"name", item_.name},
            {"production_cost", to_string(item_.productionCost)},
            {"selling_price", to_string(item_.sellingPrice)},
            {"quantity", to_string(item_.quantity)},
        };
        
        if (!db.runQuery(query, params)){
            logError("Failed to add menu (ID: " + to_string(item_.id) + ") into order_itemss (ID: " + orderID + ").");
            return false;
        }
        logInfo("Menu (ID: " + to_string(item_.id) + " added into order_items (ID: " + orderID + ").");
    }

    return true;
}

bool Order::cancelOrder(int id) {
    Database db;

    string query = "UPDATE `order` SET transaction_status = 'Cancelled' WHERE id = :id AND transaction_status = 'Completed';";
    map<string, string> params = {
        {"id", to_string(id)}
    };

    return db.runQuery(query, params);
}

TableList Order::orderList(string search, string sortColumn, bool sortAsc, int page, int limitRowPerPage) {
    Database db;
    Auth auth;

    Auth::UserDetails userDetails = Auth::retrieveLoggedUserDetails();

    vector<string> sortableColumnKeys = {"created_at", "created_by", "transaction_status", "total_items"};

    string orderQuery = "ORDER BY o.created_at DESC, o.cancelled_at DESC ";
    if (find(sortableColumnKeys.begin(), sortableColumnKeys.end(), sortColumn) != sortableColumnKeys.end()){
        if (sortColumn == "created_by"){
            sortColumn = "u.name";
        }
        else{
            sortColumn = "o." + sortColumn;
        }

        orderQuery = "ORDER BY " + sortColumn + " " + ((sortAsc) ? "ASC " : "DESC ");
    }
    
    int offset = limitRowPerPage * (page - 1);
    string limitOffsetQuery = "LIMIT " + to_string(limitRowPerPage) + " OFFSET " + to_string(offset) + ";";

    string additionalFilter = (toLowerCase(userDetails.role) != "admin") ? "AND o.created_by = " + to_string(userDetails.id) + " " : "";

    // Retrieve the total of row (Without limit)
    string query = "SELECT COUNT(*) AS total FROM `order` o "
                   "LEFT JOIN user AS u ON u.id = o.created_by "
                   "WHERE (o.created_at LIKE :search OR u.name LIKE :search OR o.transaction_status LIKE :search OR o.total_items LIKE :search) "
                   + additionalFilter;

    map<string, string> params = {
        {"search", "%" + search + "%"}
    };

    int total = stoi(db.fetchData(query, params)[0].at("total"));

    // Retrieve the rows (With limit)
    query = "SELECT o.id, o.created_at, u.name AS created_by, o.transaction_status, o.total_items FROM `order` o "
            "LEFT JOIN user AS u ON u.id = o.created_by "
            "WHERE (o.created_at LIKE :search OR u.name LIKE :search OR o.transaction_status LIKE :search OR o.total_items LIKE :search) "
            + additionalFilter + orderQuery + limitOffsetQuery;

    vector<map<string, string>> list = db.fetchData(query, params);

    return {list, total};
}

Order::OrderDetails Order::orderDetails(int id) {
    Database db;
    Order::OrderDetails details;
    Order::Item item;

    string query;
    map<string, string> params;
    vector<map<string, string>> result; 

    // Default value
    details.id = -1;
    details.createdBy = "";
    details.createdAt = "";
    details.isCompleted = false;
    details.cancelledAt = "";
    details.itemsList = {};

    // Details
    query = "SELECT o.id, o.transaction_status, o.created_by AS created_by_id, u.name AS created_by, o.created_at, o.cancelled_at "
            "FROM `order` o "
            "LEFT JOIN user AS u ON u.id = o.created_by "
            "WHERE o.id = :id LIMIT 1;";
    params = {
        {"id", to_string(id)}
    };

    result = db.fetchData(query, params);

    if (result.empty()){
        logError("Failed to retrieve order details (ID: " + to_string(id) + ").");
        return details;
    }

    details.id = id;
    details.createdBy = result[0].at("created_by");
    details.createdAt = result[0].at("created_at");
    details.isCompleted = (result[0].at("transaction_status") == "Completed");
    details.cancelledAt = result[0].at("cancelled_at");

    // Item list
    query = "SELECT * FROM order_items WHERE order_id = :order_id;";
    params = {
        {"order_id", to_string(id)}
    };

    result = db.fetchData(query, params);

    if (result.empty()){
        logError("Failed to retrieve items list. (Order ID: " + to_string(id) + ")");
        return details;
    }

    for (const auto& item_ : result) {
        item.id = stoi(item_.at("id"));
        item.name = item_.at("name");
        item.productionCost = stod(item_.at("production_cost"));
        item.sellingPrice = stod(item_.at("selling_price"));
        item.quantity = stoi(item_.at("quantity"));

        details.itemsList.push_back(item);
    }

    return details;
}
