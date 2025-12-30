#include "ui/transaction_history.h"
#include "ui/ui_manager.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

void TransactionHistoryUI::main() {
    Page page = UIManager::currentPageDetails();

    UIManager::header("Transaction History");
    UIManager::errorMessages(page.errorMessages, false);
    UIManager::infoMessages(page.infoMessages, false);

    map<string, string> params = page.params;

    UIManager::clearErrorMessages();
    UIManager::clearInfoMessages();

    /*
    switch (page.subID){
        case 1:
            
            break;

        case 2:
            
            break;

        case 0:
        default:
            
    }*/
}
