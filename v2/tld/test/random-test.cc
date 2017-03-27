
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/core/types_c.h"
#include "opencv2/highgui/highgui.hpp"

#include "common/Arguments.hpp"
#include "core/Sequence.hpp"
#include "core/Frame.hpp"
#include "core/Gaussian.hpp"

using namespace std;
using namespace cv;

Mat myFlip(Mat M) {
    Mat result = Mat();
    cv::flip(M, result, -1);
    return result;
}

// Mat* blur(Mat* img, int sigma) {
//     Mat M = *img;
//
//     cv::Mat gaussianKernel = cv::getGaussianKernel(12, 2, CV_64F);
//     cv::mulTransposed(gaussianKernel, gaussianKernel, false);
//
//     const int additionalRows = gaussianKernel.rows-1;
//     const int additionalCols = gaussianKernel.cols-1;
//
//     Mat extendedImage = Mat();
//     copyMakeBorder(M,
//                    extendedImage,
//                    (additionalRows + 1) / 2,
//                    (additionalRows) / 2,
//                    (additionalCols + 1) / 2,
//                    (additionalCols) / 2,
//                    BORDER_CONSTANT,
//                    Scalar(0));
//
//    Point anchor(gaussianKernel.cols - gaussianKernel.cols/2 - 1, gaussianKernel.rows - gaussianKernel.rows/2 - 1);
//    int borderMode = BORDER_CONSTANT;
//
//    Mat dest = Mat();
//    filter2D(extendedImage, dest, extendedImage.depth(), flip(gaussianKernel), anchor, 0, borderMode);
//    dest = dest.colRange(gaussianKernel.cols/2, dest.cols - (gaussianKernel.cols-1)/2)
//               .rowRange(gaussianKernel.rows/2, dest.rows - (gaussianKernel.rows-1)/2);
//
//    return new Mat(dest);
// }

Mat bgr2gray(Mat image) {
    Mat dest;
    cvtColor(image, dest, CV_BGR2GRAY);
    return dest;
}

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);
    Sequence sequence = Sequence(args.getString("sequence"));

    for (int i = 0; i < 1; i++) {
        while (sequence.hasNext()) {
            Frame* frame = sequence.next();
            println("%s", frame->name.c_str());

            IplImage* grayscale = frame->grayscale;
            // IplImage* img = cvLoadImage("/home/gunturkun/MasterThesis/resources/04_pedestrian2/sequence/00001.jpg", CV_LOAD_IMAGE_GRAYSCALE);
            IplImage* blurred = frame->gaussian;
            // Mat m = Mat(*blurred);

            // cvShowImage("GRAY", grayscale);
            // cvWaitKey(0);
            //
            // cvShowImage("BLUR", blurred);
            // cvWaitKey(0);

            // imshow("BLUR", blurred);
            // waitKey(0);

            delete frame;

        }
    }
}

int main1(int argc, char** argv) {
    //"/home/gunturkun/MasterThesis/resources/04_pedestrian2/sequence/00001.jpg"

    Mat colored = imread("test/resources/test.jpg", CV_LOAD_IMAGE_ANYCOLOR);
    Mat* coloredPtr = new Mat(colored);

    // Mat grayscale = bgr2gray(colored);
    IplImage* img = cvLoadImage("test/resources/test.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    // Mat grayscale = imread("test/resources/test.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    Mat grayscale = cvarrToMat(img);
    Mat* grayPtr = new Mat(grayscale);

    cv::Mat gaussKernel = cv::getGaussianKernel(12, 2, CV_64F);
    cv::Mat gaussianKernel;

    cv::mulTransposed(gaussKernel, gaussianKernel, false);

    const int additionalRows = gaussianKernel.rows-1;
    const int additionalCols = gaussianKernel.cols-1;

    Mat extendedImage = Mat();
    copyMakeBorder(grayscale,
                   extendedImage,
                   (additionalRows + 1) / 2,
                   (additionalRows) / 2,
                   (additionalCols + 1) / 2,
                   (additionalCols) / 2,
                   BORDER_CONSTANT,
                   Scalar(0));

    Mat blurred;
    Point anchor(gaussianKernel.cols - gaussianKernel.cols/2 - 1, gaussianKernel.rows - gaussianKernel.rows/2 - 1);

    filter2D(extendedImage, blurred, extendedImage.depth(), myFlip(gaussianKernel), anchor, 0, BORDER_CONSTANT);

    blurred = blurred.colRange(gaussianKernel.cols/2, blurred.cols - (gaussianKernel.cols-1)/2)
                  .rowRange(gaussianKernel.rows/2, blurred.rows - (gaussianKernel.rows-1)/2);

    long total = 0;

    for (int i = 0; i < blurred.rows; i++) {
      for (int j = 0; j < blurred.cols; j++) {
          uchar pixel = blurred.at<uchar>(i, j);
          total += (int) pixel;
      }

    }

      printf("Total(%d x %d): %10lu\n", blurred.rows, blurred.cols, total);


    printf("Hello\n");
    imshow("Gray", *grayPtr);
    imshow("Blur", blurred);

    waitKey(0);
}
