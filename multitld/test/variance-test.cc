#include "testbase/Sequence.hpp"
#include "detector/nn/Patch.hpp"

void printBoxPixels(Frame* frame, Box* box) {
    Mat* grayscale = frame->grayscale;

    for (int i = box->y1; i <= box->y2; i++) {
        for (int j = box->x1; j <= box->x2; j++) {
            uchar value = grayscale->at<uchar>(i, j);
            printf("%d ", value);
        }
        printf("\n");
    }
    printf("----------\n");
}

double computeVariance(Frame* frame, Box* box) {
    Patch* patch = new Patch(frame, box);
    Mat* grayscale = patch->data;

    vector<double> pixels;
    // for (int i = 0; i <= patch->height; i++) {
    //     for (int j = box->x1; j <= box->x2; j++) {
    //         double value = grayscale->at<double>(i, j);
    //
    //     }
    // }

    for (int i = 0; i < Patch::HEIGHT; i++) {
        for (int j = 0; j < Patch::WIDTH; j++) {
            double value = grayscale->at<double>(i,j);
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

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("tld/car");
    Frame* frame = sequence->next();

    Mat* grayscale = frame->grayscale;
    int* integralImage = frame->integral->integralImage;

    for (int i = 0; i <= 20; i++) {
        for (int j = 0; j <= 20; j++) {
            uchar value = grayscale->at<uchar>(i, j);
            printf("%5d ", value);
        }
        printf("\n");
    }

    printf("------------------------------------------------------\n");

    for (int i = 0; i <= 20; i++) {
        for (int j = 0; j <= 20; j++) {
            int value = integralImage[i * 320 + j];
            printf("%5d ", value);
        }
        printf("\n");
    }

    printf("------------------------------------------------------\n");

    Box* b2 = new Box(0, 141, 125, 231, 164);
    double variance = computeVariance(frame, b2);
    // int sum = frame->integral->computeSubWindow(b2->y1, b2->x1, b2->width, b2->height, false);
    printf("VAR >>> %g\n", variance /  4.0);

    // for (int i = 0; i < 240 * 320; i++) {
    //         int pixel = integralImage[i];
    //         if (pixel == 22748) {
    //             printf("%d\n", i);
    //         }
    // }


    // printBoxPixels(frame, b2);




    // int sum = frame->integral->computeSubWindow(b2->y1, b2->x1, b2->width, b2->height, false);
    // printf("Var: %g, %f\n", variance, meanVariance->variance);
}
