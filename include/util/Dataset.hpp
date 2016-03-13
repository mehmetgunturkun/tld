#ifndef DATASET_H
#define DATASET_H

#include "core/Box.hpp"

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "core/Frame.hpp"
#include "core/Box.hpp"

#define RESOURCE_DIR "/Users/mehmetgunturkun/MasterThesis/resources"
using namespace std;

class Dataset {
public:
    string key;
    vector<string> files;

    static vector<string> extensions;

    int size;
    int processed;

    Box* initBox;

    Dataset(string dataKey, int skip);
    Dataset(string key):Dataset(key, 0) {}

    bool hasNext();
    Frame* next();
    void reset();
};
#endif
