#include "ui/order.h"
#include "ui/ui_manager.h"
#include "core/users.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

void OrderUI::main() {
    Page page = UIManager::currentPageDetails();

    UIManager::header("Order System");
    UIManager::errorMessages(page.errorMessages, false);
    UIManager::infoMessages(page.infoMessages, false);

    map<string, string> params = page.params;

    UIManager::clearErrorMessages();
    UIManager::clearInfoMessages();

    
}
