#include <stdlib.h>
#include <stdio.h>

#include <unordered_map>

#include "common/Option.hpp"
#include "common/Logging.hpp"
using namespace std;

class CommandLineArguments {
private:
    unordered_map<string, string> argumentMap;
public:
    CommandLineArguments(int argc, char** args) {
        if ((argc - 1) % 2 != 0) {
            println("Invalid number of arguments - %2d", argc);
        } else {
            for (int i = 1; i < argc; i = i + 2) {
                string key(args[i]);
                string value(args[i + 1]);
                argumentMap[key] = value;
            }
        }
    }

    int getInt(string key) {
        return 1;
    }

    string getString(string key);

    Option<int> getIntMaybe(string key) {
        string maybeValue = argumentMap[key];
        if (maybeValue == "") {
            Option<int> none = Option<int>();
            return none;
        } else {
            int* number = (int*) malloc(sizeof(int));
            *number = stoi(maybeValue);

            Option<int> maybeInt = Option<int>(number);
            return maybeInt;
        }
    }

    Option<string> getStringMaybe(string key) {
        string maybeValue = argumentMap[key];
        if (maybeValue == "") {
            Option<string> none = Option<string>();
            return none;
        } else {
            Option<string> maybeString = Option<string>(&maybeValue);
            return maybeString;
        }
    }
};

int main(int argc, char** args) {
    CommandLineArguments* arguments = new CommandLineArguments(argc, args);

    Option<int> maybeLimit = arguments->getIntMaybe("limit");
    if (maybeLimit.isDefined()) {
        println("There is an argument with name 'limit': %d", *(maybeLimit.get()));
    } else {
        println("There is no argument with name 'limit'");
    }

    Option<string> maybeKey = arguments->getStringMaybe("key");
    if (maybeKey.isDefined()) {
        println("There is an argument with name 'key': %s", (*(maybeKey.get())).c_str());
    } else {
        println("There is no argument with name 'key'");
    }

    Option<string> maybeNonExistentKey = arguments->getStringMaybe("non-existent-key");
    if (maybeNonExistentKey.isDefined()) {
        println("There is an argument with name 'non-existent-key': %s", (*(maybeKey.get())).c_str());
    } else {
        println("There is no argument with name 'non-existent-key'");
    }

    return 0;
}
