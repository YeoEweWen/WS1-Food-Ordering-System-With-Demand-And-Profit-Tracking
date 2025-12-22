#include "ui/users.h"
#include "ui/ui_manager.h"
#include "core/users.h"
#include "core/validation.h"

using namespace std;

void UsersUI::main() {
    UIManager::header("Users Management");
    UIManager::errorMessages(UIManager::currentPageDetails().errorMessages, false);

    UIManager::emptyPage(false);
}