#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <json/json.h>
#include <json/value.h>

using namespace std;

class Config {
private:
    Json::Value root;
public:
    Config(string fileName);
    Config(Json::Value* root);

    string getString(string key);
    int getInt(string key);
    double getDouble(string key);
};


class Conf {
public:
    static void load(string fileName);
    static string getString(string key);
    static int getInt(string key);
    static double getDouble(string key);
};

#endif
