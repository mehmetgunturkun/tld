#include "common/Arguments.hpp"
#include "testbase/Sequence.hpp"
#include "detector/nn/Patch.hpp"
#include "detector/nn/ObjectModel.hpp"

void printDoubleImage(Mat* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            double p = m->at<double>(i, j);
            printf("% 02.6g ", p);
        }
        printf("\n------------------------------------------------\n");
    }
}

void buildHMatrix() {
    
}

int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);
    string sequenceKey = args->getString("sequence");
    Sequence* sequence = new Sequence(sequenceKey);

    println("There are %d frames for %s", sequence->nrOfFrames, sequenceKey.c_str());
}
