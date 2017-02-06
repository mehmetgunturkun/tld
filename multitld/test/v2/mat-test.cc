#include "core/Image.hpp"

int main(int argc, char** args) {

    Mat* image = new Mat(20, 20, CV_8U, 1);

    int rows = image->rows;
    int cols = image->cols;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            image->at<uchar>(i, j) = (uchar) i+ j;
        }
    }


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            uchar pixel = image->at<uchar>(i, j);
            int pixelValue = (int) pixel;

            printf("%2d ", pixelValue);
        }
        printf("\n");
    }

    printf("-------------------\n");

    uchar* data = image->data;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            uchar pixel = data[i * cols + j];
            int pixelValue = (int) pixel;

            printf("%2d ", pixelValue);
        }
        printf("\n");
    }


    Mat* patch = new Mat(*image, Rect(4,4,3,3));
    rows = patch->rows;
    cols = patch->cols;
    printf("-------------------\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            uchar pixel = patch->at<uchar>(i, j);
            int pixelValue = (int) pixel;

            printf("%2d ", pixelValue);
        }
        printf("\n");
    }

}
