#include "core/analytics.h"
#include "core/database.h"
#include "utils/functions.h"

using namespace std;

map<string, string> Analytics::overallSummary() {
    Database db;

    string query = "SELECT COUNT(DISTINCT o.id) AS total_orders, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN oi.quantity ELSE 0 END) AS total_items_sold, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN oi.selling_price * oi.quantity ELSE 0 END) AS total_revenue, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN oi.production_cost * oi.quantity ELSE 0 END) AS total_cost, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN (oi.selling_price - oi.production_cost) * oi.quantity ELSE 0 END) AS net_profit, "
                   "COUNT(DISTINCT CASE WHEN o.transaction_status = 'Cancelled' THEN o.id END) AS cancelled_orders, "
                   "ROUND(COUNT(DISTINCT CASE WHEN o.transaction_status = 'Completed' THEN o.id END)/ NULLIF(COUNT(DISTINCT o.id), 0) * 100, 2) AS completion_rate "
                   "FROM `order` o "
                   "JOIN order_items AS oi ON oi.order_id = o.id "
                   "WHERE o.created_at >= DATE_FORMAT(CURDATE(), '%Y-%m-01');";

    return db.fetchData(query)[0];
}

vector<map<string, string>> Analytics::monthlySalesReports() {
    Database db;

    string query = "SELECT DATE_FORMAT(o.created_at, '%Y-%m') AS month, "
                   "COUNT(DISTINCT o.id) AS total_orders, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN oi.quantity ELSE 0 END) AS total_items_sold, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN oi.selling_price * oi.quantity ELSE 0 END) AS total_revenue, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN oi.production_cost * oi.quantity ELSE 0 END) AS total_cost, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN (oi.selling_price - oi.production_cost) * oi.quantity ELSE 0 END) AS net_profit "
                   "FROM `order` o "
                   "JOIN order_items AS oi ON oi.order_id = o.id "
                   "WHERE o.created_at >= DATE_SUB(CURDATE(), INTERVAL 12 MONTH) "
                   "GROUP BY month ORDER BY month DESC;";

    vector<map<string, string>> result = db.fetchData(query);

    for (auto& i : result){
        i["month"] = getProperMonthYear(i.at("month"));
    }

    return result;
}

vector<map<string, string>> Analytics::demandsTracking(string yearMonth) {
    Database db;

    string query = "SELECT oi.name AS menu, SUM(oi.quantity) AS quantity_sold, SUM(oi.selling_price * oi.quantity) AS total_revenue "
                   "FROM `order` o "
                   "JOIN order_items AS oi ON oi.order_id = o.id "
                   "WHERE o.created_at >= STR_TO_DATE(CONCAT(:created_at,'-01'), '%Y-%m-%d') AND "
                   "o.created_at < DATE_ADD(STR_TO_DATE(CONCAT(:created_at,'-01'), '%Y-%m-%d'), INTERVAL 1 MONTH) AND "
                   "o.transaction_status = 'Completed' "
                   "GROUP BY menu ORDER BY quantity_sold DESC LIMIT 10;";

    map<string, string> params = {
        {"created_at", yearMonth}
    };

    return db.fetchData(query, params);
}

vector<map<string, string>> Analytics::profitsTracking(string yearMonth) {
    Database db;

    string query = "SELECT oi.name AS menu, SUM(oi.selling_price * oi.quantity) AS total_revenue, "
                   "SUM((oi.selling_price - oi.production_cost) * oi.quantity) AS net_profit, "
                   "ROUND(SUM((oi.selling_price * oi.quantity) - (oi.production_cost * oi.quantity)) / NULLIF(SUM(oi.selling_price * oi.quantity), 0) * 100, 2) AS profit_margin "
                   "FROM `order` o "
                   "JOIN order_items AS oi ON oi.order_id = o.id "
                   "WHERE o.created_at >= STR_TO_DATE(CONCAT(:created_at,'-01'), '%Y-%m-%d') AND "
                   "o.created_at < DATE_ADD(STR_TO_DATE(CONCAT(:created_at,'-01'), '%Y-%m-%d'), INTERVAL 1 MONTH) AND "
                   "o.transaction_status = 'Completed' "
                   "GROUP BY menu ORDER BY net_profit DESC LIMIT 10;";

    map<string, string> params = {
        {"created_at", yearMonth}
    };

    return db.fetchData(query, params);
}

vector<map<string, string>> Analytics::categoryPerformance(string yearMonth) {
    Database db;

    string query = "SELECT mc.category AS category, mc.type AS type, COUNT(DISTINCT o.id) AS total_orders, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN oi.selling_price * oi.quantity ELSE 0 END) AS total_revenue, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN oi.quantity ELSE 0 END) AS quantity_sold "
                   "FROM `order` o "
                   "JOIN order_items AS oi ON oi.order_id = o.id "
                   "JOIN menu AS m ON m.id = oi.menu_id "
                   "JOIN menu_category AS mc ON mc.id = m.category_id "
                   "WHERE o.created_at >= STR_TO_DATE(CONCAT(:created_at,'-01'), '%Y-%m-%d') AND "
                   "o.created_at < DATE_ADD(STR_TO_DATE(CONCAT(:created_at,'-01'), '%Y-%m-%d'), INTERVAL 1 MONTH) "
                   "GROUP BY category, type ORDER BY quantity_sold DESC, total_revenue DESC;";

    map<string, string> params = {
        {"created_at", yearMonth}
    };

    return db.fetchData(query, params);
}

vector<map<string, string>> Analytics::employeePerformance(string yearMonth) {
    Database db;

    string query = "SELECT u.name AS employee_name, COUNT(DISTINCT o.id) AS total_orders, "
                   "SUM(CASE WHEN o.transaction_status = 'Completed' THEN oi.selling_price * oi.quantity ELSE 0 END) AS total_revenue, "
                   "ROUND(COUNT(DISTINCT CASE WHEN o.transaction_status = 'Completed' THEN o.id END)/ NULLIF(COUNT(DISTINCT o.id), 0) * 100, 2) AS completion_rate "
                   "FROM `order` o "
                   "JOIN user AS u ON u.id = o.created_by "
                   "JOIN order_items AS oi ON oi.order_id = o.id "
                   "WHERE o.created_at >= STR_TO_DATE(CONCAT(:created_at,'-01'), '%Y-%m-%d') AND "
                   "o.created_at < DATE_ADD(STR_TO_DATE(CONCAT(:created_at,'-01'), '%Y-%m-%d'), INTERVAL 1 MONTH) AND "
                   "u.role = 'Employee' AND u.status = 'Active' "
                   "GROUP BY employee_name ORDER BY total_orders DESC, total_revenue DESC;";

    map<string, string> params = {
        {"created_at", yearMonth}
    };

    return db.fetchData(query, params);
}
