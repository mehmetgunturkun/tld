#ifndef BASE_CLASSIFIER_H
#define BASE_CLASSIFIER_H

#include "detector/ensemble/PixelComparison.hpp"
#include "detector/ensemble/CodeGenerator.hpp"
#include "detector/ensemble/DecisionTree.hpp"

class BaseClassifier {
    CodeGenerator* codeGen;
    DecisionTree* decTree;
public:
    int id;
    BaseClassifier(int i, vector<PixelComparison*> comparisons, int nrOfModels);

    int generateBinaryCode(Frame* frame, Box* box);
    double getProbability(int binaryCode, int modelId);
    void train(int binaryCode, int modelId, bool label);
};

#endif
