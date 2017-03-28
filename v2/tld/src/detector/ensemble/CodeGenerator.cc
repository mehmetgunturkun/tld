#include "detector/ensemble/CodeGenerator.hpp"

CodeGenerator::CodeGenerator(vector<PixelComparison*> pixelComparisons) {
    this->pixelComparisons = pixelComparisons;
    this->nrOfComparisons = (int) pixelComparisons.size();
}

CodeGenerator::~CodeGenerator() {
    for (int i = 0; i < nrOfComparisons; i++) {
        PixelComparison* comp = pixelComparisons[i];
        printf("%s\n", comp->toString().c_str());
        delete comp;
    }
}

int CodeGenerator::generateBinaryCode(Frame* frame, Box* box) {
    int binaryCode = 0;
    for (int i = 0 ; i < nrOfComparisons; i++) {
        binaryCode <<= 1;
        PixelComparison* comparison = pixelComparisons[i];
        bool bit = comparison->compare(frame, box);
        if (bit) {
            binaryCode |= 1;
        }
    }
    return binaryCode;
}

void CodeGenerator::dumpCodeGeneration() {
    for (int i = 0 ; i < nrOfComparisons; i++) {
        PixelComparison* comparison = pixelComparisons[i];
        printf("%s\n", comparison->toString().c_str());
    }
}
