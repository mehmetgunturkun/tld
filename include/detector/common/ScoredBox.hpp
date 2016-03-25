#ifndef SCORED_BOX_HPP
#define SCORED_BOX_HPP

#include "core/Box.hpp"
#include <string>
#include <unordered_map>

using namespace std;

class ScoredBox {
public:
    Box* box;
    unordered_map<string, double> scoreMap;

    ScoredBox(Box* box);

    void withScore(string scorerKey, double score);
};
#endif
