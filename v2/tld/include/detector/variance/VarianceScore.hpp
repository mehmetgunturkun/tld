#ifndef VARIANCE_SCORE_H
#define VARIANCE_SCORE_H

#include "core/Score.hpp"

class VarianceScore: public Score {
    Score* clone() {
        VarianceScore* varianceScore = new VarianceScore();
        return varianceScore;
    }

    double getValue(int modelId) {
        return 0.0;
    }

    Score* sum(Score* other) {
        return (Score*) this;
    }

    Score* divide(int n) {
        return (Score*) this;
    }
};
#endif
