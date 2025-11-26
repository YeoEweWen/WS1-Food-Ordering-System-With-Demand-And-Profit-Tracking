#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <string>
#include <vector>
#include <mysql.h>
#include <iostream>

class Database {
private:
    // Variables
    MYSQL* conn;
    MYSQL_RES* retrievedData;
    std::string host;
    std::string username;
    std::string password;
    std::string dbName;
    int port;

    // Methods
    bool initConnection();
    void closeConnection();
    std::string generateCompletedQuery(std::string query, const std::map<std::string, std::string> &params);
    std::map<std::string, std::string> rowToMap(MYSQL_ROW row, MYSQL_FIELD* fields, int numFields);

public:
    Database();           // constructor

    // Methods
    bool runQuery(std::string query, const std::map<std::string, std::string> &params = {});

    std::vector<std::map<std::string, std::string>> fetchData(std::string query, const std::map<std::string, std::string> &params = {});

};

#endif
