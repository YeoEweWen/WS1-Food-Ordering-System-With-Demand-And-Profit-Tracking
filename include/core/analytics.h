#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>

class Analytics {
public:
    std::map<std::string, std::string> overallSummary();
    std::vector<std::map<std::string, std::string>> monthlySalesReports();
    std::vector<std::map<std::string, std::string>> demandsTracking(std::string yearMonth);
    std::vector<std::map<std::string, std::string>> profitsTracking(std::string yearMonth);
    std::vector<std::map<std::string, std::string>> categoryPerformance(std::string yearMonth);
    std::vector<std::map<std::string, std::string>> employeePerformance(std::string yearMonth);

};

#endif
