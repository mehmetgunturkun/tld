#ifndef BASE_CLASSIFIER_H
#define BASE_CLASSIFIER_H

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/ScoredBox.hpp"

#include "detector/ensemble/EnsembleScore.hpp"
#include "detector/ensemble/PixelComparison.hpp"
#include "detector/ensemble/CodeGenerator.hpp"
#include "detector/ensemble/DecisionTree.hpp"

class BaseClassifier {
    CodeGenerator* codeGen;
    DecisionTree* decTree;
public:
    int id;
    BaseClassifier(int i, vector<PixelComparison*> comparisons);
    vector<float> score(Frame* frame, Box* box, EnsembleScore* score);
    void train(Frame* frame, Box* box, int modelId, bool label);
    void train(Frame* frame, ScoredBox* box, int modelId, bool label);
};
#endif