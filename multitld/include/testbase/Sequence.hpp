#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "core/Box.hpp"
#include "core/Frame.hpp"

#define RESOURCE_DIR "/Users/mehmetgunturkun/MasterThesis/resources"

using namespace std;

class Sequence {
public:
    string key;
    vector<string> files;
    Box* initBox;

    int processedFrames;
    int nrOfFrames;

    Sequence(string key, int skip = 0);
    bool hasNext();
    Frame* next();
    Frame* get(int frameNo);

    static vector<string> extensions;
};
#endif
