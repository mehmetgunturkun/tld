#include "common/Config.hpp"
#include "common/Option.hpp"

Config::Config(string fileName) {
    Json::Reader reader;
    std::ifstream file(fileName, std::ifstream::binary);
    reader.parse(file, root, false );
}

Config::Config(Json::Value* json) {
    root = *json;
}

Json::Value Config::getJsonValue(string key) {
    vector<string> keys = StringUtils::split(key, '.');
    Json::Value valuePtr = root[keys[0]];
    for (int i = 1; i < (int) keys.size(); i++) {
        valuePtr = valuePtr[keys[i]];
    }
    return valuePtr;
}

string Config::getString(string key, string defaultVal) {
    Json::Value jsonValue = getJsonValue(key);
    string value = jsonValue.asString();
    return value;
}

int Config::getInt(string key, int defaultVal) {
    Json::Value jsonValue = getJsonValue(key);
    int value = jsonValue.asInt();
    return value;
}

double Config::getDouble(string key, double defaultVal) {
    Json::Value jsonValue = getJsonValue(key);
    double value = jsonValue.asDouble();
    return value;
}

Config* Config::getConfig(string key) {
    Json::Value jsonValue = getJsonValue(key);
    Config* config = new Config(&jsonValue);
    return config;
}

Config* Conf::root = (Config*) NULL;
