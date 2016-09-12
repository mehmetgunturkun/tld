#ifndef VARIANCE_SCORE_H
#define VARIANCE_SCORE_H

#include "core/Score.hpp"

class VarianceScore: public Score {
    Score* clone() {
        VarianceScore* varianceScore = new VarianceScore();
        return varianceScore;
    }
};
#endif
