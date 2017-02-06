#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "common/Arguments.hpp"
#include "common/Logging.hpp"

#include "detector/nn/Patch.hpp"

#include "testbase/Sequence.hpp"


using namespace std;
int run(Arguments* arguments);
void printPatch(Patch* patch);

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
    println("Fetching sequence for %s", sequenceKey.c_str());
    Sequence* sequence = new Sequence(sequenceKey);

    Frame* firstFrame = sequence->next();
    Box* initBox = new Box(0, 141, 124, 186, 163);

    Patch* patch = new Patch(firstFrame, initBox);
    printPatch(patch);

    free(patch);
    return 0;
}

void printPatch(Patch* patch) {
    int height = Patch::HEIGHT;
    int width = Patch::WIDTH;

    Mat* data = patch->data;

    println("-------------------------------------------------------------------------------------------------------------------------------------------");
    printf("%4s", "");

    for (int j = 0; j < width; j++) {
        printf("%8d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < height; i++) {
        printf(" %2d ", i + 1);
        for (int j = 0; j < width; j++) {
            double p = data->at<double>(i, j);
            printf("%8.4f ", p);
        }
        printf("\n");
    }
    println("-------------------------------------------------------------------------------------------------------------------------------------------");
}
