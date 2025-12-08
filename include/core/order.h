#ifndef ORDER_H
#define ORDER_H

#include "core/database.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>

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

    // Order
    bool createOrder();
    bool cancelOrder(int id);
    std::vector<std::map<std::string, std::string>> orderList(int createdBy = 0);
    OrderDetails orderDetails(int id);

private:
    std::vector<Item> items;
};

#endif