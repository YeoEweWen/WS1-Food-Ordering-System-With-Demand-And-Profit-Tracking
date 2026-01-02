#ifndef TRANSACTION_HISTORY_UI_H
#define TRANSACTION_HISTORY_UI_H

#include <string>
#include <iostream>
#include "ui/ui_manager.h"

class TransactionHistoryUI {
public:
    static void main();
    static void list(std::string search="", Sort sort={-1, true}, int pageNum=1, int maxPerPage=25);
    static void details(int id);

private:
    
};

#endif