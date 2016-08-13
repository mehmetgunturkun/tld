#ifndef NN_SCORE_H
#define NN_SCORE_H

#include <vector>

#include "core/Score.hpp"
#include "detector/nn/Patch.hpp"

using namespace std;

class NNScore: public Score {
public:
    Patch* patch;
    vector<float> relativeScores;
    vector<float> conservativeScores;

    NNScore(Patch* patch, vector<float> relativeScores, vector<float> conservativeScores);

};
#endif
