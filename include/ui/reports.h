#ifndef REPORTS_UI_H
#define REPORTS_UI_H

#include <string>
#include <iostream>
#include "ui/ui_manager.h"

class ReportUI {
public:
    static void main();

private:
    static void overallSummary();
    static void monthlySalesReports();
    static void mostDemands(std::string yearMonth = "");
    static void mostProfitable(std::string yearMonth = "");
    static void categoryPerformance(std::string yearMonth = "");
    static void employeePerformance(std::string yearMonth = "");
};

#endif