#include "core/Score.hpp"

Score::~Score() {}

double Score::getValue(int modelId) {
    throw runtime_error("not implemented: Score.getValue");
    return 0.0;
}

Score* Score::sum(Score* other) {
    throw runtime_error("not implemented: Score.sum");
    return this;
}

Score* Score::divide(int n) {
    throw runtime_error("not implemented: Score.divide");
    return this;
}

Score* Score::clone() {
    throw runtime_error("not implemented: Score.clone");
    return this;
}
