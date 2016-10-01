#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H

#include "opencv2/imgproc/imgproc.hpp"
#include "core/Image.hpp"
using namespace cv;


class Gaussian {
public:
    static Mat flip(Mat M) {
        Mat result = Mat();
        cv::flip(M, result, -1);
        return result;
    }

    static Mat* blur(Image* img, int sigma) {
        Mat M = img->underlying->clone();

        cv::Mat gaussianKernel = cv::getGaussianKernel(12, 2, CV_64F);
        cv::mulTransposed(gaussianKernel, gaussianKernel, false);

        const int additionalRows = gaussianKernel.rows-1;
        const int additionalCols = gaussianKernel.cols-1;

        Mat extendedImage = Mat();
        copyMakeBorder(M,
                       extendedImage,
                       (additionalRows + 1) / 2,
                       (additionalRows) / 2,
                       (additionalCols + 1) / 2,
                       (additionalCols) / 2,
                       BORDER_CONSTANT,
                       Scalar(0));

       Point anchor(gaussianKernel.cols - gaussianKernel.cols/2 - 1, gaussianKernel.rows - gaussianKernel.rows/2 - 1);
       int borderMode = BORDER_CONSTANT;

       Mat dest = Mat();
       filter2D(extendedImage, dest, extendedImage.depth(), Gaussian::flip(gaussianKernel), anchor, 0, borderMode);
       dest = dest.colRange(gaussianKernel.cols/2, dest.cols - (gaussianKernel.cols-1)/2)
                  .rowRange(gaussianKernel.rows/2, dest.rows - (gaussianKernel.rows-1)/2);

       return new Mat(dest);
    }
};
#endif
