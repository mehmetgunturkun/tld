#include "common/Config.hpp"

Config::Config(string fileName) {
    Json::Reader reader;
    std::ifstream file(fileName, std::ifstream::binary);
    reader.parse(file, root, false );
}

Config::Config(Json::Value* jValue) {
    root = *jValue;
}

vector<string> split(const std::string &s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


Json::Value getJsonValue(Json::Value root, string key) {
    StringStream stream = StringStream(key, '.');

    Json::Value valuePtr = root;
    while (stream.hasNext()) {
        string subKey = stream.next();
        valuePtr = valuePtr[subKey];
    }

    return valuePtr;
}

string Config::getString(string key) {
    Json::Value jsonValue = getJsonValue(root, key);
    string value = jsonValue.asString();
    return value;
}

int Config::getInt(string key) {
    Json::Value jsonValue = getJsonValue(root, key);
    int value = jsonValue.asInt();
    return value;
}

double Config::getDouble(string key) {
    Json::Value jsonValue = getJsonValue(root, key);
    double value = jsonValue.asDouble();
    return value;
}

Config* root = (Config*) NULL;


void Conf::load(string fileName) {
    root = new Config(fileName);
}

string Conf::getString(string key) {
    return root->getString(key);
}

int Conf::getInt(string key) {
    return root->getInt(key);
}

double Conf::getDouble(string key) {
    return root->getDouble(key);
}
