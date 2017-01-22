#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int main(int argc, char** argv) {
    Mat* m1 = new Mat(3, 3, CV_64F, 0.0);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m1->at<double>(i, j) = i + j;
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double p = m1->at<double>(i, j);
            printf("%g ", p);
        }
        printf("\n");
    }
    printf("\n");

    Mat* m2 = new Mat();
    m1->copyTo(*m2);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m2->at<double>(i, j) = i + j * 10;
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double p = m1->at<double>(i, j);
            printf("%g ", p);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double p = m2->at<double>(i, j);
            printf("%g ", p);
        }
        printf("\n");
    }

    return 0;
}
