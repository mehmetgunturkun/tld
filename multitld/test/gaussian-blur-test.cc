#include "core/Image.hpp"
#include "core/Frame.hpp"
#include "core/Gaussian.hpp"

void printIntegerImage(Mat* m) {
    for (int i = 220; i < 240; i++) {
        for (int j = 300; j < 320; j++) {
            int p = (int) m->at<uchar>(i, j);
            printf("%3d ", p);
        }
        printf("\n");
    }
}

void printDoubleImage(Mat* m) {
    for (int i = 220; i < 240; i++) {
        for (int j = 300; j < 320; j++) {
            double p = m->at<double>(i, j);
            printf("%g ", p);
        }
        printf("\n");
    }
}

int main(int argc, char** args) {
    Frame* jpgImage = new Frame("resources/test.jpg");
    Mat* testImage = jpgImage->grayscale;
    printIntegerImage(testImage);

    Mat* blurredImage = Gaussian::blur(testImage, 6);
    printf("%d, %d\n", blurredImage->cols, blurredImage->rows);
    printIntegerImage(blurredImage);
    return 0;
}
