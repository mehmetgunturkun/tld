#include "core/Score.hpp"

Score::~Score() {}

double Score::getValue(int modelId) {
    throw "NotImplemented";
    return 0.0;
}

Score* Score::sum(Score* other) {
    throw "NotImplemented";
    return this;
}

Score* Score::divide(int n) {
    throw "NotImplemented";
    return this;
}

Score* Score::clone() {
    throw "NotImplemented";
    return this;
}
