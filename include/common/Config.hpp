#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <json/json.h>
#include <json/value.h>

#include "common/StringUtils.hpp"
#include "util/Util.h"

using namespace std;

class Config {
private:
    Json::Value root;
    Json::Value getJsonValue(string key);
public:
    Config(string fileName);
    Config(Json::Value* json);

    string getString(string key,  string defaultVal = "__NAN__");
    int getInt(string key,  int defaultVal);
    double getDouble(string key, double defaultVal);
    Config* getConfig(string key);
};

class Conf {
private:
    static Config* root;
public:
    static void load(string fileName) {
        root = new Config(fileName);
    }

    static string getString(string key,  string defaultVal = "__NAN__") {
        return root->getString(key, defaultVal);
    }

    static int getInt(string key,  int defaultVal) {
        return root->getInt(key, defaultVal);
    }

    static double getDouble(string key, double defaultVal) {
        return root->getDouble(key, defaultVal);;
    }
};
#endif
