#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

class Config {
public:
    static std::map<std::string, std::string> load(const std::string &filePath);
};

#endif
