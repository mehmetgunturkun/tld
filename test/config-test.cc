#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>

#include "common/Config.hpp"

int main(int argc, char** argv) {

    Config* config = new Config("resources/tld.json");

    string stringVal = config->getString("testString");
    printf("STR: %s\n", stringVal.c_str());

    int intVal = config->getInt("testInteger", -1);
    printf("INT: %d\n", intVal);

    stringVal = config->getString("testObject.testInnerString");
    printf("STR: %s\n", stringVal.c_str());

    intVal = config->getInt("testObject.testInnerInteger", -1);
    printf("INT: %d\n", intVal);

    Config* objectConf = config->getConfig("testObject");

    intVal = objectConf->getInt("testInnerInteger", -1);
    printf("INT: %d\n", intVal);

    return 0;
}
