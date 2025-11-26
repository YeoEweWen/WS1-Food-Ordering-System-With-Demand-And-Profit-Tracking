#include "config.h"
#include <fstream>
#include <sstream>

std::map<std::string, std::string> Config::load(const std::string &filePath) {
    std::map<std::string, std::string> settings;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip empty and comments
        
        std::stringstream ss(line);
        std::string key, value;

        if (std::getline(ss, key, '=') && std::getline(ss, value)) {
            settings[key] = value;
        }
    }

    return settings;
}
