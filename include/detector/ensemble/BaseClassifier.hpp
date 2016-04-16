#ifndef BASE_CLASSIFIER_H
#define BASE_CLASSIFIER_H

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/common/ScoredBox.hpp"

#include "detector/ensemble/Leaf.hpp"
#include "detector/ensemble/PixelComparison.hpp"
#include "detector/ensemble/EnsembleClassificationDetails.hpp"

using namespace std;

class BaseClassifier {
private:
    vector<PixelComparison*> pixelComparisons;
    int nrOfComparisons;
    vector<Leaf*> decisionTree;
public:
    int id;
    BaseClassifier(int i, vector<PixelComparison*> comparisons);
    void init(Frame* frame, Box* box, bool label);
    double score(Frame* frame, Box* box);
    void update(Frame* frame, ScoredBox* box, bool label);

    int generateBinaryCode(Frame* frame, Box* box);
    double getProbability(int binaryCode);

    string toString() {
        stringstream ss;
        ss << "BC(id = " << id << ")\n";
        for (int i = 0; i < nrOfComparisons; i++) {
            PixelComparison* pc = pixelComparisons[i];
            ss << "\t"
               << pc->toString()
               << "\n";
        }
        return ss.str();
    }
};

#endif
