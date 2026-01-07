#ifndef ORDERS_UI_H
#define ORDERS_UI_H

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include "ui/ui_manager.h"

class OrderUI {
public:
    static void main();

private:
    static std::map<int, std::map<std::string, std::string>> mappedMenuPriceList;
    static std::map<int, std::string> categoryList;
    static std::map<int, std::vector<int>> categorizedMenuPriceList;

    static void clearStoredMenuAndCart();
};

#endif