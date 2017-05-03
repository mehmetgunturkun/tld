#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <unordered_map>
#include <string>

#include "common/Logging.hpp"

using namespace std;

class Arguments {
private:
    unordered_map<string, string> argumentMap;
public:
    Arguments(int argc, char** args);
    int getInt(string key);
    int getIntOrDefault(string key, int defaultVal);

    string getString(string key);
    string getStringOrDefault(string key, string defaultVal);
};
#endif
