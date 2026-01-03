#ifndef ORDER_H
#define ORDER_H

#include "core/database.h"
#include "utils/functions.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>

class Order {     
public:
    Order(); // constructor

    struct Item
    {
        int id;
        std::string name;
        double productionCost;
        double sellingPrice;
        int quantity;
    };

    struct OrderDetails
    {
        int id;
        std::string createdBy;
        std::string createdAt;
        bool isCompleted;
        std::string cancelledAt;
        std::vector<Item> itemsList;
    };

    // Items
    void clearItems();
    void addItem(int id, std::string name, double productionCost, double sellingPrice, int quantity);
    void updateQuantity(int id, int newQuantity);
    void removeItems(int id);
    const std::vector<Item>& itemsList() const;

    // Order / Transaction
    bool createOrder();
    bool cancelOrder(int id);
    TableList orderList(std::string search="", std::string sortColumn="", bool sortAsc=true, int page=1, int limitRowPerPage=25);
    OrderDetails orderDetails(int id);

private:
    std::vector<Item> items;
};

#endif