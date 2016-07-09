#include "core/Image.hpp"

int main(int argc, char** args) {
    // ------------------------------------------------------------------------------------------
    Mat* jpgImage = Image::imread("resources/test.jpg");
    printf("Image(JPG) type: %d, #channels: %d\n", jpgImage->type(), jpgImage->channels());
    Image::imshow("jpg - 0", jpgImage, 1000);

    Mat* jpgImageGray1 = Image::rgb2gray(jpgImage);
    printf("Image(JPG-Gray) type: %d, #channels: %d\n", jpgImageGray1->type(), jpgImageGray1->channels());
    Image::imshow("jpg - 1", jpgImageGray1, 1000);

    Mat* jpgImageGray2 = Image::bgr2gray(jpgImage);
    printf("Image(JPG-Gray) type: %d, #channels: %d\n", jpgImageGray2->type(), jpgImageGray2->channels());
    Image::imshow("jpg - 2", jpgImageGray2, 1000);

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
