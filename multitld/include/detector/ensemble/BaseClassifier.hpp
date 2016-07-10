#ifndef BASE_CLASSIFIER_H
#define BASE_CLASSIFIER_H

#include "core/Frame.hpp"
#include "core/ScoredBox.hpp"

#include "detector/ensemble/PixelComparison.hpp"
#include "detector/ensemble/CodeGenerator.hpp"
#include "detector/ensemble/DecisionTree.hpp"

class BaseClassifier {
    CodeGenerator* codeGen;
    DecisionTree* decTree;

    int id;
    BaseClassifier(int i, vector<PixelComparison*> comparisons);
    vector<float> score(Frame* frame, ScoredBox* box);
};
#endif
