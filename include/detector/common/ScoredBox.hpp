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
    bool isDetected;
    unordered_map<string, ClassificationDetails*> detailMap;

    ScoredBox(Box* box);

    void withScore(string scorerKey, ClassificationDetails* detail);

    double getScore(string scorerKey) {
        ClassificationDetails* details = detailMap[scorerKey];
        return details->score;
    }

    ClassificationDetails* getDetail(string scorerKey) {
        ClassificationDetails* details = detailMap[scorerKey];
        return details;
    }
};

struct ScoredBoxOverlapOrdered {
  bool operator() (ScoredBox* box1, ScoredBox* box2) {
      return box1->box->overlap >= box2->box->overlap;
  }
};
#endif
