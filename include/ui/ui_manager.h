#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>

struct Page {
        int id;
        int subID;
        std::map<std::string, std::string> params;
        std::vector<std::string> errorMessages;
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
    static void errorMessages(const std::vector<std::string>& errorMessages, bool topBorder = true, bool bottomBorder = true);
    static void emptyPage(bool includeHeader=true);
};

#endif
