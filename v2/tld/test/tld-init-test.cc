#include <iostream>
#include <fstream>

#include "common/Arguments.hpp"
#include "core/Sequence.hpp"
#include "tld/TLD.hpp"

int main(int argc, char** args) {
    Arguments arguments = Arguments(argc, args);

    string key = arguments.getString("sequence");
    int limit = arguments.getIntOrDefault("limit", 0);

    printf("Going to run for %s\n", key.c_str());
    Sequence sequence = Sequence(key, 0, limit);
    Frame* firstFrame = sequence.next();
    Box* initBox = sequence.initBox;
    initBox->isValid = true;
    vector<Box*> boxList = { initBox };


    TLD* tld = new TLD();
    boxList = tld->init(firstFrame, boxList);

    return EXIT_SUCCESS;
}
