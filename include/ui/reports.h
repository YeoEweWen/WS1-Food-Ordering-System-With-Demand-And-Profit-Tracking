#ifndef REPORTS_UI_H
#define REPORTS_UI_H

#include <string>
#include <iostream>
#include "ui/ui_manager.h"

class ReportUI {
public:
    static void main();

private:
    static void overall();
    static void mostDemands();
    static void mostProfitable();
    static void monthlySalesReport();
    static void categoryPerformance();
};

#endif