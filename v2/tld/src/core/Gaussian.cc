#include "core/Gaussian.hpp"

Mat flipMat(Mat& mat) {
    Mat result = Mat();
    cv::flip(mat, result, -1);
    return result;
}

IplImage* Gaussian::blur(IplImage* img, int kernelSize, double sigma) {
    Mat image = cvarrToMat(img);

    cv::Mat gaussKernel = cv::getGaussianKernel(kernelSize, sigma, CV_64F);
    cv::Mat gaussianKernel;
    cv::mulTransposed(gaussKernel, gaussianKernel, false);

    const int additionalRows = gaussianKernel.rows-1;
    const int additionalCols = gaussianKernel.cols-1;

    Mat extendedImage = Mat();
    copyMakeBorder(image,
                   extendedImage,
                   (additionalRows + 1) / 2,
                   (additionalRows) / 2,
                   (additionalCols + 1) / 2,
                   (additionalCols) / 2,
                   BORDER_CONSTANT,
                   Scalar(0));

    Mat* blurred = new Mat();
    Point anchor(gaussianKernel.cols - gaussianKernel.cols/2 - 1, gaussianKernel.rows - gaussianKernel.rows/2 - 1);

    filter2D(extendedImage, *blurred, extendedImage.depth(), flipMat(gaussianKernel), anchor, 0, BORDER_CONSTANT);

    *blurred = blurred->colRange(gaussianKernel.cols/2, blurred->cols - (gaussianKernel.cols-1)/2);
    *blurred = blurred->rowRange(gaussianKernel.rows/2, blurred->rows - (gaussianKernel.rows-1)/2);
                 
    IplImage* blurredImage = new IplImage(*blurred);
    return blurredImage;
}
