#include "testbase/Sequence.hpp"

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
    Mat* grayscale = frame->grayscale;

    vector<int> pixels;
    for (int i = box->y1; i <= box->y2; i++) {
        for (int j = box->x1; j <= box->x2; j++) {
            uchar value = grayscale->at<uchar>(i, j);
            pixels.push_back((int) value);
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

    Box* b2 = new Box(0, 1, 1, 63, 28);
    int sum = frame->integral->computeSubWindow(b2->y1, b2->x1, b2->width, b2->height, false);
    printf("SUM >>> %d\n", sum);

    // for (int i = 0; i < 240 * 320; i++) {
    //         int pixel = integralImage[i];
    //         if (pixel == 22748) {
    //             printf("%d\n", i);
    //         }
    // }


    // printBoxPixels(frame, b2);
    // double variance = computeVariance(frame, b2);



    // int sum = frame->integral->computeSubWindow(b2->y1, b2->x1, b2->width, b2->height, false);
    // printf("Var: %g, %f\n", variance, meanVariance->variance);
}
