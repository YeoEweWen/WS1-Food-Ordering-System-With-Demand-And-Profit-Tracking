#include "core/config.h"
#include "utils/functions.h"

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

int countStringLength(const string& text){
    size_t len = text.length();
    return static_cast<int>(len);
}

void printVectorValues(const vector<map<string, string>>& mapValue){
    // Display the retrieved rows
    int i = 0;
    if (!mapValue.empty()){
        for (const auto& row : mapValue) {
            i++;
            cout << "---- ROW " << i << " ----" << endl;
            for (const auto& kv : row) {
                cout << kv.first << " = " << kv.second << endl;
            }
            cout << endl;
        }
    }
    else{
        cout << "\nNo Record Found\n" << endl;
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

string toUpperCase(const string& input) {
    string result = input;
    for (auto& c : result) {
        c = toupper(static_cast<unsigned char>(c));
    }
    return result;
}

string toLowerCase(const string& input) {
    string result = input;
    for (auto& c : result) {
        c = tolower(static_cast<unsigned char>(c));
    }
    return result;
}

vector<string> split(const string &text, char delimiter) {
    vector<string> result;
    string temp;

    for (char c : text) {
        if (c == delimiter) {
            result.push_back(temp);
            temp.clear();
        } else {
            temp += c;
        }
    }

    // Push the last part
    result.push_back(temp);

    return result;
}

string trim(const string& s) {
    auto start = s.begin();
    while (start != s.end() && isspace(static_cast<unsigned char>(*start))) {
        ++start;
    }

    auto end = s.end();
    do {
        --end;
    } while (end != start && isspace(static_cast<unsigned char>(*end)));

    return string(start, end + 1);
}

string formatDecimalPoints(double value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

