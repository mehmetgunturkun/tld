#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "common/Arguments.hpp"
#include "common/Logging.hpp"

#include "detector/Detector.hpp"

#include "testbase/Sequence.hpp"


using namespace std;
int run(Arguments* arguments);

int main(int argc, char** args) {
    Arguments* arguments = new Arguments(argc, args);
    try {
        run(arguments);
    } catch(exception& e) {
        cout << "Standard exception: " << e.what() << endl;
        return 1;
    }
}

int run(Arguments* arguments) {
    vector<string> sequences = Sequence::tldSequences;

    int nrOfSequences = (int) sequences.size();
    for (int i = 0; i < nrOfSequences; i++) {
        string sequenceKey = sequences[i];
        println(GREEN("Initiating Detector for Sequence(%s)"), sequenceKey.c_str());
        Sequence* sequence = new Sequence(sequenceKey);

        Frame* firstFrame = sequence->next();
        Box* initBox = sequence->initBox;

        vector<Box*> boxList = { initBox };
        Detector* detector = new Detector(firstFrame, boxList);
        detector->init(firstFrame, boxList);
    }

    return 0;
}
