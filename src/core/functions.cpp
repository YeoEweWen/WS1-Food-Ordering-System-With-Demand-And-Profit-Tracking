#include "config.h"
#include "functions.h"

using namespace std;

// ----- Logger -----
void logError(string message){
    auto cfg = Config::load("config/app.conf");
    string debugMode = cfg["debug_mode"];

    if (debugMode == "ON" || debugMode == "on"){
        ofstream out("logs/error.log", ios::app);
        out << "[" << timestamp() << "]   " << message << "\n";
    } 
}

void logInfo(string message){
    auto cfg = Config::load("config/app.conf");
    string debugMode = cfg["debug_mode"];

    if (debugMode == "ON" || debugMode == "on"){
        ofstream out("logs/info.log", ios::app);
        out << "[" << timestamp() << "]   " << message << "\n";
    }
}

// ----- Universal -----
string timestamp(){
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return buf;
}

bool isInteger(const string& s) {
    if (s.empty()) return false;
    try {
        size_t pos;
        stoi(s, &pos);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}

bool isFloat(const string& s) {
    if (s.empty()) return false;
    try {
        size_t pos;
        stod(s, &pos);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}

void printVectorValues(const vector<map<string, string>>& mapValue){
    // Display the retrieved rows
    for (const auto& row : mapValue) {
        cout << "---- ROW ----" << endl;
        for (const auto& kv : row) {
            cout << kv.first << " = " << kv.second << endl;
        }
        cout << endl;
    }
}

string toProperCase(const string& input) {
    string result = input;

    bool newWord = true; // flag to detect the first character of a word
    for (auto& c : result) {
        if (isspace(static_cast<unsigned char>(c))) {
            newWord = true;  // next character is new word
        } else {
            if (newWord) {
                c = toupper(static_cast<unsigned char>(c));
                newWord = false;
            } else {
                c = tolower(static_cast<unsigned char>(c));
            }
        }
    }

    return result;
}

