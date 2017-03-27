#include "common/Arguments.hpp"
#include "core/Sequence.hpp"
#include "core/Frame.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);
    Sequence sequence = Sequence(args.getString("sequence"));

    Frame* frame = sequence.next();
    Box* initBox = sequence.initBox;
    IplImage* grayscale = frame->grayscale;

    double x1 = initBox->x1;
    double y1 = initBox->y1;

    double x2 = initBox->x2;
    double y2 = initBox->y2;

    IplImage* _img = Image::warp(grayscale, x1, y1, x2, y2);

    Mat originalImage = cvarrToMat(grayscale);
    Mat warpedImage = cvarrToMat(_img);

    for (int i = 0; i < warpedImage.rows; i++) {
        for (int j = 0; j < warpedImage.cols; j++) {
            double p = warpedImage.at<double>(i, j);
            int pint = (int) round(p);
            originalImage.at<uchar>(i + y1, j + x1) = (uchar) pint;
        }
    }

    imshow("WARPED", originalImage);
    waitKey(0);

}
