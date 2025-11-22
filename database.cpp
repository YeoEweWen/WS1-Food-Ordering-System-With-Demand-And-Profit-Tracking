#include "database.h"
#include "functions.h"

using namespace std;

Database::Database() {
    conn = mysql_init(NULL);
    retrievedData = nullptr;
    host = "localhost";
    username = "root";
    password = "root";
    dbName = "sample_db";
    port = 3306;
}

Database::~Database() {
    closeConnection();
}

bool Database::initConnection() {
    // Connect to database
    if (!mysql_real_connect(conn, host.c_str(), username.c_str(), password.c_str(), dbName.c_str(), port, NULL, 0)) {
        cerr << "MySQL connection error: " << mysql_error(conn) << endl;
        return false;
    }

    return true;
}

void Database::closeConnection() {
    mysql_free_result(retrievedData);
    mysql_close(conn);
}

string Database::generateCompletedQuery(string query, const map<string, string> &params) {
    if (params.size() > 0){
        string toReplace, value;
        for (auto& p : params) {
            size_t pos = 0;
            toReplace = ':' + p.first;
            value = p.second;
            
            if (!(isInteger(value) || isFloat(value))){ // Later add filter, to avoid sql injection
                value = "'" + value + "'";
            }

            while ((pos = query.find(toReplace, pos)) != std::string::npos) {
                query.replace(pos, toReplace.length(), value);
                pos += value.length();
            }
        }
    }

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
    bool isSuccess = (mysql_query(conn, query.c_str()) == 0);

    closeConnection();

    return isSuccess;
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