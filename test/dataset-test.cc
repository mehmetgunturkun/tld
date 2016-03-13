#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>

#include "util/Dataset.hpp"

int main(int argc, char** argv) {
    Dataset* dataset = new Dataset("car");
    while(dataset->hasNext()) {
        Frame* currentFrame = dataset->next();
        string frameStr = currentFrame->toString();

        printf("%s\n", frameStr.c_str());
    }
    return 0;
}
