#include "common/Arguments.hpp"

Arguments::Arguments(int argc, char** args) {
    if ((argc - 1) % 2 != 0) {
        println("Invalid number of arguments - %2d", argc);
    } else {
        for (int i = 1; i < argc; i = i + 2) {
            string key(args[i]);
            key = key.substr(2, key.size());
            string value(args[i + 1]);
            argumentMap[key] = value;
        }
    }
}

int Arguments::getInt(string key) {
    string value = argumentMap[key];
    if (value == "") {
        throw invalid_argument("there is no item for key: " + key);
    } else {
        int valueInt = stoi(value);
        return valueInt;
    }
}

int Arguments::getIntOrDefault(string key, int defaultVal) {
    string value = argumentMap[key];
    if (value == "") {
        return defaultVal;
    } else {
        int valueInt = stoi(value);
        return valueInt;
    }
}

string Arguments::getString(string key) {
    string value = argumentMap[key];
    if (value == "") {
        throw invalid_argument("there is no item for key: " + key);
    } else {
        return value;
    }
}

string Arguments::getStringOrDefault(string key, string defaultVal) {
    string value = argumentMap[key];
    if (value == "") {
        return defaultVal;
    } else {
        return value;
    }
}
