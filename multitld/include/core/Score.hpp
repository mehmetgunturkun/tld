#ifndef SCORE_H
#define SCORE_H

#include <vector>

#include "core/TLDConfig.hpp"

using namespace std;

class Score {
public:
    double value;
    virtual Score* merge(Score* other);

    bool isAnyModellClassified;
    vector<int> classifiedModelIds;
};
#endif
