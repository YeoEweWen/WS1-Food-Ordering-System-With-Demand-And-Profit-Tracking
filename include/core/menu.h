#ifndef MENU_H
#define MENU_H

#include "utils/functions.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>

class Menu {     
private:
    bool isMenuDeletable(int id);

public:
    struct MenuDetails {
        int id;
        std::string name = "";
        double productionCost = 0;
        double sellingPrice = 0;
        double profitMargin = 0;
        std::string category = "";
        std::string type = "";
        std::string availability = "";
        std::string createdAt = "";
        int createdByID = -1;
        std::string createdByName = "";
        std::string updatedAt = "";
        int updatedByID = -1;
        std::string updatedByName = "";
        bool deletable = false;
    };

    // Menu Category
    bool addMenuCategory(std::string categoryName);
    bool updateMenuCategory(int id, std::string categoryName, std::string type);
    std::vector<std::map<std::string, std::string>> categoryList(bool detailedList = false);

    // Menu
    bool addMenu(std::string name, double productionCost, double sellingPrice, int categoryID);
    bool updateMenu(int id, std::string name, double productionCost, double sellingPrice, int categoryID);
    bool deleteMenu(int id);
    double calculateProfitMargin(double productionCost, double sellingPrice);
    bool setAvailable(int id);
    bool setUnavailable(int id);
    TableList menuList(std::string search="", std::string sortColumn="", bool sortAsc=true, int page=1, int limitRowPerPage=25);
    MenuDetails menuDetails(int id);
    std::vector<MenuDetails> menuPriceList();
};

#endif