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

    query = "INSERT INTO order ";

    return false;
}

bool Order::cancelOrder(int id) {
    Database db;

    string query = "UPDATE order SET transaction_status = 'Cancelled' WHERE id = :id AND transaction_status = 'Completed';";
    map<string, string> params = {
        {"id", to_string(id)}
    };

    return db.runQuery(query, params);
}

vector<map<string, string>> Order::orderList(int createdBy) {
    Database db;

    string query = "SELECT o.id, o.transaction_status, o.created_by AS created_by_id, u.name AS created_by, o.created_at, o.cancelled_at "
                   "FROM order o "
                   "LEFT JOIN user AS u ON u.id = o.created_by "
                   "WHERE o.created_by = :created_by OR :created_by = 0 "
                   "ORDER BY o.transaction_status DESC, o.created_at DESC;";
    
    map<string, string> params = {
        {"created_by", to_string(createdBy)}
    };

    return db.fetchData(query, params);
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
            "FROM order o "
            "LEFT JOIN user AS u ON u.id = o.created_by "
            "WHERE id = :id LIMIT 1;";
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
