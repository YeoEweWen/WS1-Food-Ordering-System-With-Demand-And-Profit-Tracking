#ifndef ORDERS_UI_H
#define ORDERS_UI_H

#include <string>
#include <iostream>
#include "ui/ui_manager.h"

class OrderUI {
public:
    static void main();

private:
    static void list(std::string search="", Sort sort={-1, true}, int pageNum=1, int maxPerPage=25);
    static void details(int id);
    
};

#endif