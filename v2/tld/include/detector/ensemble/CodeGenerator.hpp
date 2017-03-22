#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "detector/ensemble/PixelComparison.hpp"

class CodeGenerator {
private:
    vector<PixelComparison*> pixelComparisons;
    int nrOfComparisons;
public:
    CodeGenerator(vector<PixelComparison*> pixelComparisons);
    int generateBinaryCode(Frame* frame, Box* box);
    void printPixelComparison();
};

#endif
