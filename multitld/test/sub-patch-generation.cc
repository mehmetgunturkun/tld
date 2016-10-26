#include <stdlib.h>
#include <stdio.h>

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "testbase/Sequence.hpp"
#include "detector/nn/Patch.hpp"

double computeVariance(Patch* patch) {
    vector<double> pixels;
    for (int i = 0; i < Patch::HEIGHT; i++) {
        for (int j = 0; j < Patch::WIDTH; j++) {
            double value = patch->data->at<double>(i,j);
            pixels.push_back(value);
        }
    }

    int nrOfPixels = (int) pixels.size();

    double sum = 0.0;
    for (int i = 0; i < nrOfPixels; i++) {
        sum += pixels[i];
    }
    double mean = sum / nrOfPixels;
    double variance = 0.0;
    for (int i = 0; i < nrOfPixels; i++) {
        variance += pow(pixels[i] - mean, 2);
    }

    variance = variance / nrOfPixels;
    return variance;
}

int main(int argc, char** args) {
    Sequence* sequence = new Sequence("tld/car");

    Frame* firstFrame = sequence->next();
    Frame* secondFrame = sequence->next();

    Box* b0 = new Box(0, 141, 125, 231, 164);
    printf("B0 is standard: %d\n", b0->isStandard());
    Patch* p0 = new Patch(firstFrame, b0);

    Mat* m = p0->data;
    for (int j = 0; j < m->cols; j++) {
        for (int i = 0; i < m->rows; i++) {

            printf("%g ",(double) m->at<double>(i,j));
        }
        printf("\n------------\n");
    }


    double var = computeVariance(p0);
    printf("Variance: %g\n", var);

    // Box* b1 = new Box(0, 137.228, 124.577, 227.049, 163.499);
    // printf("B1 is standard: %d\n", b1->isStandard());
    // Patch* p1 = new Patch(secondFrame, b1);
    //
    // Mat* m = p1->data;
    // for (int i = 0; i < m->rows; i++) {
    //     for (int j = 0; j < m->cols; j++) {
    //         printf("%g ",(double) m->at<double>(i,j));
    //     }
    //     printf("\n------------\n");
    // }

}
