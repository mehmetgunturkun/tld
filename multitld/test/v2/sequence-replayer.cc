#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>

#include "common/Arguments.hpp"
#include "testbase/Sequence.hpp"
#include "util/ImageBuilder.hpp"
#include "common/Logging.hpp"

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
    string sequenceKey = arguments->getString("sequence");
    println(GREEN("Replaying Sequence(%s)"), sequenceKey.c_str());
    Sequence* sequence = new Sequence(sequenceKey);
    int frameIndex = 0;

    vector<Box*> groundTruth = sequence->loadGroundTruth();

    while (sequence->hasNext()) {
        Frame* frame = sequence->next();
        Box* box = groundTruth[frameIndex];

        ImageBuilder* builder = new ImageBuilder(frame);
        builder->withBox(box, Colors::YELLOW);
        builder->display(5);
        frameIndex += 1;
    }

    return 0;
}
