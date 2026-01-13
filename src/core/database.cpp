#include "core/config.h"
#include "core/database.h"
#include "utils/functions.h"

using namespace std;

Database::Database() {
    retrievedData = nullptr;
    
    auto cfg = Config::load("config/app.conf");

    host     = cfg["db_host"];
    username = cfg["db_username"];
    password = cfg["db_password"];
    dbName   = cfg["db_database"];
    port     = std::stoi(cfg["db_port"]);
}

bool Database::initConnection() {
    conn = mysql_init(NULL);

    // Connect to database
    if (!mysql_real_connect(conn, host.c_str(), username.c_str(), password.c_str(), dbName.c_str(), port, NULL, 0)) {
        logError("MySQL connection error: " + string(mysql_error(conn)));
        return false;
    }

    return true;
}

void Database::closeConnection() {
    if (retrievedData != nullptr) {
        mysql_free_result(retrievedData);
        retrievedData = nullptr;
    }
    if (conn != nullptr) {
        mysql_close(conn);
        conn = nullptr;
    }
}

string Database::generateCompletedQuery(string query, const map<string, string> &params) {
    if (params.size() > 0){
        string toReplace, value;
        for (auto& p : params) {
            size_t pos = 0;
            toReplace = ':' + p.first;
            value = p.second;
            
            if (!(isInteger(value) || isFloat(value) || value == "NULL")){ // Later add filter, to avoid sql injection, check for :parameter with no provided value
                value = "'" + value + "'";
            }

            while ((pos = query.find(toReplace, pos)) != std::string::npos) {
                query.replace(pos, toReplace.length(), value);
                pos += value.length();
            }
        }
    }
    logInfo("MYSQL Query: " + query);

    return query;
};

map<string, string> Database::rowToMap(MYSQL_ROW row, MYSQL_FIELD* fields, int numFields){
    map<string, string> record;

    for (int i = 0; i < numFields; i++) {
        string colName  = fields[i].name;           // column name
        string colValue = row[i] ? row[i] : "NULL"; // row value

        record[colName] = colValue;
    }

    return record;
}

bool Database::runQuery(string query, const map<string, string> &params) {
    if (!initConnection()) {
        return false;
    }

    query = generateCompletedQuery(query, params);

    if (mysql_query(conn, query.c_str()) != 0){
        logError("MySQL Error: " + string(mysql_error(conn)));
        closeConnection();

        return false;
    }
    closeConnection();

    return true;
}

vector<map<string, string>> Database::fetchData(string query, const map<string, string> &params) {
    vector<map<string, string>> results;

    if (!initConnection()) {
        return {};
    }

    query = generateCompletedQuery(query, params);

    if (mysql_query(conn, query.c_str()) != 0) {
        closeConnection();
        return {};
    }

    retrievedData = mysql_store_result(conn);
    if (!retrievedData) {
        logError("MySQL Error: " + string(mysql_error(conn)));
        closeConnection();
        return {};
    }

    int numFields = mysql_num_fields(retrievedData);
    MYSQL_FIELD* fields = mysql_fetch_fields(retrievedData);

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(retrievedData))) {
        results.push_back(rowToMap(row, fields, numFields));
    }

    closeConnection();

    return results;
}