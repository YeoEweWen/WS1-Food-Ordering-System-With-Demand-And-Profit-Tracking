#ifndef STORE_H
#define STORE_H

#include <string>
#include <iostream>
#include <map>
#include <vector>

class Store {
public:
    struct StoreDetails {
        std::string name;
        std::string createdAt;
        std::string createdBy;
        std::string updatedAt;
        std::string updatedBy;
    };

    static StoreDetails details();
    static bool update(std::string storeName);
};

#endif
