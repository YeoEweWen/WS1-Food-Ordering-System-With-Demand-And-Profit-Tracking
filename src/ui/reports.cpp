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
        if (params.count("year_month")){
            selectedYearMonth = params.at("year_month");
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
    Analytics analytics;

    map<string, string> summary = analytics.overallSummary();

    cout<<"\nCurrent Month Overview"<<endl;
    cout<<string(UIManager::getlineLength(), '-')<<endl;
    
    cout<<"Total Orders            : "<<summary.at("total_orders")<<endl;
    cout<<"Total Items Sold        : "<<summary.at("total_items_sold")<<endl;
    cout<<"Total Revenue           : RM "<<summary.at("total_revenue")<<endl;
    cout<<"Total Cost              : RM "<<summary.at("total_cost")<<endl;
    cout<<"Net Profit              : RM "<<summary.at("net_profit")<<endl;
    cout<<"Total Cancelled Orders  : "<<summary.at("cancelled_orders")<<endl;
    cout<<"Completion Rate         : "<<summary.at("completion_rate")<<"%"<<endl;

    cout<<string(UIManager::getlineLength(), '-')<<endl;

    cout<<"\n[1] Monthly Sales Reports"<<endl;
    cout<<"[2] Most Demands Items"<<endl;
    cout<<"[3] Most Profitable Items"<<endl;
    cout<<"[4] Category Performance"<<endl;
    cout<<"[5] Employee Performance"<<endl;

    cout<<"\n[X] Back"<<endl;

    cout<<"\nSelect Option > ";
    string option;
    getline(cin, option);

    if (option == "1"){
        // Monthly Sales Reports
        UIManager::goTo(6, 1);
    }
    else if (option == "2"){
        // Most Demands
        UIManager::goTo(6, 2);
    }
    else if (option == "3"){
        // Most Profitable
        UIManager::goTo(6, 3);
    }
    else if (option == "4"){
        // Category Performance
        UIManager::goTo(6, 4);
    }
    else if (option == "5"){
        // Employee Performance
        UIManager::goTo(6, 5);
    }
    else if (option == "x" || option == "X"){
        UIManager::goTo(1);
        UIManager::clearParams();
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
    
}

void ReportUI::monthlySalesReports() {
    Analytics analytics;

    vector<map<string, string>> reports = analytics.monthlySalesReports();

    cout<<endl;
    UIManager::baseTable(
        "Monthly Sales Reports",
        {
            {"month", "Month"},
            {"total_orders", "Total Orders"},
            {"total_items_sold", "Total Items Sold"},
            {"total_revenue", "Total Revenue", "RM "},
            {"total_cost", "Total Cost", "RM "},
            {"net_profit", "Net Profit", "RM "},
        },
        reports
    );

    cout<<"\n[X] Back"<<endl;

    cout<<"\nSelect Option > ";
    string option;
    getline(cin, option);

    if (toUpperCase(option) == "X"){
        UIManager::goTo(6);
        UIManager::clearParams();
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}

void ReportUI::mostDemands(string yearMonth) {
    Analytics analytics;

    string currentMonth = timestamp().substr(0, 7);
    yearMonth = (yearMonth == "") ? currentMonth : yearMonth;

    vector<map<string, string>> reports = analytics.demandsTracking(yearMonth);

    cout<<endl<<string(((UIManager::getlineLength() - 18) / 2), ' ')<<"Most Demands Items"<<endl;
    UIManager::baseTable(
        getProperMonthYear(yearMonth),
        {
            {"menu", "Menu"},
            {"quantity_sold", "Quantity Sold"},
            {"total_revenue", "Total Revenue", "RM "},
        },
        reports
    );

    cout<<"\nActions:"<<endl;

    if (!reports.empty()){
        cout<<"[P] Previous Month"<<endl;
    }
    if (yearMonth != currentMonth){
        cout<<"[N] Next Month"<<endl;
    }
    cout<<"[X] Back"<<endl;

    cout<<"\nSelect Option > ";
    string option;
    getline(cin, option);

    if (toUpperCase(option) == "P" && !reports.empty()){
        UIManager::addParam("year_month", getPreviousMonth(yearMonth));
    }
    else if (toUpperCase(option) == "N" && yearMonth != currentMonth){
        UIManager::addParam("year_month", getNextMonth(yearMonth));
    }
    else if (toUpperCase(option) == "X"){
        UIManager::goTo(6);
        UIManager::clearParams();
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}

void ReportUI::mostProfitable(string yearMonth) {
    Analytics analytics;

    string currentMonth = timestamp().substr(0, 7);
    yearMonth = (yearMonth == "") ? currentMonth : yearMonth;

    vector<map<string, string>> reports = analytics.profitsTracking(yearMonth);

    cout<<endl<<string(((UIManager::getlineLength() - 18) / 2), ' ')<<"Most Profitable Items"<<endl;
    UIManager::baseTable(
        getProperMonthYear(yearMonth),
        {
            {"menu", "Menu"},
            {"total_revenue", "Total Revenue", "RM "},
            {"net_profit", "Net Profit", "RM "},
            {"profit_margin", "Profit Margin", "", "%"},
        },
        reports
    );

    cout<<"\nActions:"<<endl;

    if (!reports.empty()){
        cout<<"[P] Previous Month"<<endl;
    }
    if (yearMonth != currentMonth){
        cout<<"[N] Next Month"<<endl;
    }
    cout<<"[X] Back"<<endl;

    cout<<"\nSelect Option > ";
    string option;
    getline(cin, option);

    if (toUpperCase(option) == "P" && !reports.empty()){
        UIManager::addParam("year_month", getPreviousMonth(yearMonth));
    }
    else if (toUpperCase(option) == "N" && yearMonth != currentMonth){
        UIManager::addParam("year_month", getNextMonth(yearMonth));
    }
    else if (toUpperCase(option) == "X"){
        UIManager::goTo(6);
        UIManager::clearParams();
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}

void ReportUI::categoryPerformance(string yearMonth) {
    Analytics analytics;

    string currentMonth = timestamp().substr(0, 7);
    yearMonth = (yearMonth == "") ? currentMonth : yearMonth;

    vector<map<string, string>> reports = analytics.categoryPerformance(yearMonth);

    cout<<endl<<string(((UIManager::getlineLength() - 18) / 2), ' ')<<"Category Performance"<<endl;
    UIManager::baseTable(
        getProperMonthYear(yearMonth),
        {
            {"category", "Category"},
            {"type", "Type"},
            {"total_orders", "Total Orders"},
            {"total_revenue", "Total Revenue", "RM "},
            {"quantity_sold", "Quantity Sold"}
        },
        reports
    );

    cout<<"\nActions:"<<endl;

    if (!reports.empty()){
        cout<<"[P] Previous Month"<<endl;
    }
    if (yearMonth != currentMonth){
        cout<<"[N] Next Month"<<endl;
    }
    cout<<"[X] Back"<<endl;

    cout<<"\nSelect Option > ";
    string option;
    getline(cin, option);

    if (toUpperCase(option) == "P" && !reports.empty()){
        UIManager::addParam("year_month", getPreviousMonth(yearMonth));
    }
    else if (toUpperCase(option) == "N" && yearMonth != currentMonth){
        UIManager::addParam("year_month", getNextMonth(yearMonth));
    }
    else if (toUpperCase(option) == "X"){
        UIManager::goTo(6);
        UIManager::clearParams();
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}

void ReportUI::employeePerformance(string yearMonth) {
    Analytics analytics;

    string currentMonth = timestamp().substr(0, 7);
    yearMonth = (yearMonth == "") ? currentMonth : yearMonth;

    vector<map<string, string>> reports = analytics.employeePerformance(yearMonth);

    cout<<endl<<string(((UIManager::getlineLength() - 18) / 2), ' ')<<"Employee Performance"<<endl;
    UIManager::baseTable(
        getProperMonthYear(yearMonth),
        {
            {"employee_name", "Employee Name"},
            {"total_orders", "Total Orders"},
            {"total_revenue", "Total Revenue", "RM "},
            {"completion_rate", "Completion Rate", "", "%"},
        },
        reports
    );

    cout<<"\nActions:"<<endl;

    if (!reports.empty()){
        cout<<"[P] Previous Month"<<endl;
    }
    if (yearMonth != currentMonth){
        cout<<"[N] Next Month"<<endl;
    }
    cout<<"[X] Back"<<endl;

    cout<<"\nSelect Option > ";
    string option;
    getline(cin, option);

    if (toUpperCase(option) == "P" && !reports.empty()){
        UIManager::addParam("year_month", getPreviousMonth(yearMonth));
    }
    else if (toUpperCase(option) == "N" && yearMonth != currentMonth){
        UIManager::addParam("year_month", getNextMonth(yearMonth));
    }
    else if (toUpperCase(option) == "X"){
        UIManager::goTo(6);
        UIManager::clearParams();
    }
    else{
        UIManager::addErrorMessage("Invalid Option.");
    }
}