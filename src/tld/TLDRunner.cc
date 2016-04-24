#include "tld/TLDRunner.hpp"


TLDRunner::TLDRunner(string key) {
    Conf::load("resources/tld.json");
    dataset = new Dataset("car");

    firstFrame = dataset->next();
    firstBox = dataset->initBox;

    tld = new TLD(firstFrame, firstBox);
}

void TLDRunner::run() {
    Option<Box>* maybeFirstBox = new Option<Box>(firstBox);
    while(dataset->hasNext()) {
        Frame* secondFrame = dataset->next();
        Option<Box>* trackResult = tld->process(firstFrame, secondFrame, maybeFirstBox);

        firstFrame = secondFrame;
        maybeFirstBox = trackResult;
    }
}

void TLD::persistOutput(Option<Box>* maybeBox) {
    
}
