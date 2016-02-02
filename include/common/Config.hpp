#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <json/json.h>
#include <json/value.h>

#include "common/StringUtils.hpp"

using namespace std;

class Config {
private:
    Json::Value root;
    Json::Value getJsonValue(string key);
public:
    Config(string fileName);
    Config(Json::Value* json);

    string getString(string key);
    int getInt(string key);
    Config* getConfig(string key);
};
#endif
