#include "detector/ensemble/CodeGenerator.hpp"

CodeGenerator::CodeGenerator(vector<PixelComparison*> pixelComparisons) {
    this->pixelComparisons = pixelComparisons;
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
