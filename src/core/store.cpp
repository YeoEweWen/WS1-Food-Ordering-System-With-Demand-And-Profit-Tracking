#include "core/store.h"
#include "core/database.h"
#include "utils/functions.h"

using namespace std;

Store::StoreDetails Store::details(){
    Database db;

    string query = "SELECT si.store_name, si.created_at, u.name AS created_by, si.updated_at, u2.name AS updated_by FROM store_info si "
                   "LEFT JOIN user AS u ON u.id = si.created_by "
                   "LEFT JOIN user AS u2 ON u2.id = si.updated_by "
                   "WHERE si.id = 1 LIMIT 1;";
    map<string, string> details = db.fetchData(query)[0];

    return {
        details.at("store_name"),
        details.at("created_at"),
        details.at("created_by"),
        ((details.at("updated_at") == "NULL") ? "-" : details.at("updated_at")),
        ((details.at("updated_by") == "NULL") ? "-" : details.at("updated_by"))
    };
}

bool Store::update(string storeName){
    Database db;

    string query = "UPDATE store_info SET store_name = :store_name WHERE id = 1;";
    map<string, string> params = {{"store_name", storeName}};

    return db.runQuery(query, params);
}
