#ifndef SCORE_H
#define SCORE_H

#include <vector>

using namespace std;

class Score {
public:
    bool isAnyModellClassified;
    vector<int> classifiedModelIds;

    virtual ~Score();
    virtual double getValue(int modelId);
};
#endif
