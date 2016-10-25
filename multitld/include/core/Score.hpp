#ifndef SCORE_H
#define SCORE_H

#include <vector>

#include "core/TLDConfig.hpp"

using namespace std;

class Score {
public:
    double value;
    bool isAnyModellClassified;
    vector<int> classifiedModelIds;

    virtual Score* sum(Score* other);
    virtual Score* divide(int n);
    virtual Score* clone();
    virtual double getValue(int modelId);
};
#endif
