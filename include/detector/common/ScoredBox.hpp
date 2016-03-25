#ifndef SCORED_BOX_HPP
#define SCORED_BOX_HPP

#include "core/Box.hpp"
#include <string>
#include <unordered_map>
#include "detector/common/ClassificationDetails.hpp"

using namespace std;

class ScoredBox {
public:
    Box* box;
    unordered_map<string, ClassificationDetails*> detailMap;

    ScoredBox(Box* box);

    void withScore(string scorerKey, ClassificationDetails* detail);
};
#endif
