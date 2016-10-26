#include "core/Image.hpp"

int main(int argc, char** args) {
    int h = 40;
    int w = 91;

    Mat* matrix = new Mat(h, w, CV_8U, 0.0);
    printf("%d \n", (int) matrix->at<uchar>(0, 0));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            // printf("%d \n", m->at<uchar>(i, j));
            // m->at<uchar>(i, j) = (int) round(result[i * w + j]);
            printf("%d, %d\n", i, j);
            int myPixel = (int) matrix->data[0];
            printf("%d, %d, %d\n", i, j, myPixel);

        }
        printf("\n--------------\n");
    }

    // ------------------------------------------------------------------------------------------
    Mat* jpgImage = Image::imread("resources/test.jpg");
    printf("Image(JPG) type: %d, #channels: %d\n", jpgImage->type(), jpgImage->channels());

    Mat* jpgImageGray1 = Image::rgb2gray(jpgImage);
    printf("Image(JPG-Gray) type: %d, #channels: %d\n", jpgImageGray1->type(), jpgImageGray1->channels());

    Mat* jpgImageGray2 = Image::bgr2gray(jpgImage);
    printf("Image(JPG-Gray) type: %d, #channels: %d\n", jpgImageGray2->type(), jpgImageGray2->channels());

    // ------------------------------------------------------------------------------------------

    Mat* pgmImage = Image::imread("resources/test.pgm");
    printf("Image(PGM) type: %d, #channels: %d\n", pgmImage->type(), pgmImage->channels());

    Mat* pgmImageGray = Image::rgb2gray(pgmImage);
    printf("Image(PGM-Gray) type: %d, #channels: %d\n", pgmImageGray->type(), pgmImageGray->channels());

    // ------------------------------------------------------------------------------------------

    Mat* coloredImage = Image::imread("resources/colored-test.jpg");
    printf("Image(COLOR) type: %d, #channels: %d\n", coloredImage->type(), coloredImage->channels());

    Mat* coloredImageGray = Image::rgb2gray(coloredImage);
    printf("Image(Colored-Gray) type: %d, #channels: %d\n", coloredImageGray->type(), coloredImageGray->channels());

    // ------------------------------------------------------------------------------------------
}
