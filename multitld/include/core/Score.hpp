#ifndef SCORE_H
#define SCORE_H

#include <vector>

using namespace std;

class Score {
public:
    double value;
    Score* merge(Score* other);

    bool isAnyModellClassified;
    vector<int> classifiedModelIds;
};
#endif
