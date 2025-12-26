#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cmath>
#include "utils/functions.h"

struct Page {
    int id;
    int subID;
    std::map<std::string, std::string> params;
    std::vector<std::string> errorMessages;
};

struct Attribute {
    std::string key;
    std::string label;
    int maxLength = 0;
};

struct Sort {
    int columnIndex;
    bool ascendingOrder;
};

struct TableNavPermission {
    bool previosPage;
    bool nextPage;
    bool goToPage;
};

class UIManager {
protected:
    static int lineLength;
    static Page currentPage;

public:
    static Page currentPageDetails();
    static void goTo(int id, int subID=0);
    static void clearParams();
    static void clearErrorMessages();
    static void addParam(std::string key, std::string value);
    static void addErrorMessage(std::string errorMessage);
    
    static void header(const std::string& pageName = "");
    static TableNavPermission table(std::vector<Attribute> columns, std::vector<std::map<std::string, std::string>> rows, int totalRows, std::string search = "", Sort sort = {-1, true}, int pageNum=1, int maxRowPerPage=25);
    static void errorMessages(const std::vector<std::string>& errorMessages, bool topBorder = true, bool bottomBorder = true);
    static void emptyPage(bool includeHeader=true);
};

#endif
