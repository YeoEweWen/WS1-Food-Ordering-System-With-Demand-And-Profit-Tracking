#include "ui/reports.h"
#include "ui/ui_manager.h"
#include "core/analytics.h"
#include "core/validation.h"
#include "utils/functions.h"

using namespace std;

void ReportUI::main() {
    Page page = UIManager::currentPageDetails();

    UIManager::header("Analysis & Reports");
    UIManager::errorMessages(page.errorMessages, false);
    UIManager::infoMessages(page.infoMessages, false);

    map<string, string> params = page.params;

    UIManager::clearErrorMessages();
    UIManager::clearInfoMessages();

    string selectedYearMonth = "";
    if (!params.empty()){
        if (params.count("yearMonth")){
            selectedYearMonth = params.at("yearMonth");
        }
    }

    switch (page.subID){
        case 1:
            monthlySalesReports();
            break;

        case 2:
            mostDemands(selectedYearMonth);
            break;

        case 3:
            mostProfitable(selectedYearMonth);
            break;

        case 4:
            categoryPerformance(selectedYearMonth);
            break;

        case 5:
            employeePerformance(selectedYearMonth);
            break;

        case 0:
        default:
            overallSummary();
    }
}

void ReportUI::overallSummary() {

}

void ReportUI::monthlySalesReports() {

}

void ReportUI::mostDemands(string yearMonth) {

}

void ReportUI::mostProfitable(string yearMonth) {

}

void ReportUI::categoryPerformance(string yearMonth) {

}

void ReportUI::employeePerformance(string yearMonth) {

}