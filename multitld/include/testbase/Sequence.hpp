#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "core/Box.hpp"
#include "core/Frame.hpp"

// #define RESOURCE_DIR "/Users/mehmetgunturkun/MasterThesis/resources"
#define RESOURCE_DIR "/Users/mehmetgunturkun/MasterThesis/ceng500-mscthesis/resources/tld"

using namespace std;

class Sequence {
public:
    string dir;
    string key;
    vector<string> files;
    string outputFile;
    Box* initBox;

    int processedFrames;
    int nrOfFrames;
    int limit;

    Sequence(string key, int skip = 0, int limit = 0);
    bool hasNext();
    Frame* next();
    Frame* get(int frameNo);

    static vector<string> extensions;
};
#endif
