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
    bool isCategoryDeletable(int id);
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

    struct CategoryDetails {
        int id;
        std::string name = "";
        std::string type = "";
        std::string createdAt = "";
        int createdByID = -1;
        std::string createdByName = "";
        std::string updatedAt = "";
        int updatedByID = -1;
        std::string updatedByName = "";
        bool deletable = false;
    };

    // Menu Category
    bool addCategory(std::string categoryName, std::string type);
    bool updateCategoryName(int id, std::string name);
    bool updateCategoryType(int id, std::string type);
    bool deleteCategory(int id);
    std::vector<std::map<std::string, std::string>> categoryList();
    CategoryDetails categoryDetails(int id);

    // Menu
    bool addMenu(std::string name, double productionCost, double sellingPrice, int categoryID);
    bool updateMenuName(int id, std::string name);
    bool updateMenuCostAndPrice(int id, double productionCost, double sellingPrice);
    bool updateMenuCategory(int id, int categoryID);
    bool deleteMenu(int id);
    double calculateProfitMargin(double productionCost, double sellingPrice);
    bool setAvailable(int id);
    bool setUnavailable(int id);
    TableList menuList(std::string search="", std::string sortColumn="", bool sortAsc=true, int page=1, int limitRowPerPage=25);
    MenuDetails menuDetails(int id);
    std::vector<std::map<std::string, std::string>> menuPriceList();
};

#endif