#ifndef MENU_H
#define MENU_H

#include "core/database.h"
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
    // Menu Category
    bool addMenuCategory(std::string categoryName);
    bool updateMenuCategory(int id, std::string categoryName);
    std::vector<std::map<std::string, std::string>> categoryList(bool detailedList = false);

    // Menu
    bool addMenu(std::string name, double productionCost, double sellingPrice, int categoryID);
    bool updateMenu(int id, std::string name, double productionCost, double sellingPrice, int categoryID);
    bool deleteMenu(int id);
    double calculateProfitMargin(double productionCost, double sellingPrice);
    bool setAvailable(int id);
    bool setUnavailable(int id);
    std::vector<std::map<std::string, std::string>> menuList(bool detailedList = false, bool availableOnly = false);
};

#endif