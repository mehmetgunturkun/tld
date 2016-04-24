#include "util/Dataset.hpp"
#include "tld/TLD.hpp"

int main(int argc, char** argv) {
    Conf::load("resources/tld.json");
    Dataset* dataset = new Dataset("car");

    Frame* firstFrame = dataset->next();
    Box* firstBox = dataset->initBox;


    TLD* tld = new TLD(firstFrame, firstBox);
    
    while(dataset->hasNext()) {
        printf("BOX >> %s\n", firstBox->toString().c_str());
        Frame* secondFrame = dataset->next();
        Option<Box>* trackResult = tld->process(firstFrame, secondFrame, new Option<Box>(firstBox));
        if (trackResult->isEmpty()) {
            return EXIT_FAILURE;
        } else {
            firstBox = trackResult->get();
            firstFrame = secondFrame;
        }
    }
    return EXIT_SUCCESS;
}
