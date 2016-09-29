#include "core/Frame.hpp"

// Mat* blur(Mat image, double sigma) {
//     Mat* dest = new Mat(image.cols, image.rows, CV_64F);
//
//     cv::Mat gaussianKernel = cv::getGaussianKernel(12, 2, CV_32F);
//     cv::mulTransposed(gaussianKernel,gaussianKernel,false);
//
//     Mat kernel = cv::getGaussianKernel(12, sigma);
//     printf("----------------------------------------\n");
//     for (int i = 0; i < gaussianKernel.rows; i++) {
//         for (int j = 0; j < gaussianKernel.cols; j++) {
//             double d = gaussianKernel.at<double>(i, j);
//             printf("%0.5g ", d);
//         }
//         printf("\n");
//     }
//     printf("----------------------------------------\n");
//
//     Mat kernelTranspose = Mat(kernel.cols, kernel.rows, kernel.type());
//     cv::transpose(kernel, kernelTranspose);
//     Mat kernel2d = kernel * kernelTranspose;
//
//     Mat* dest = new Mat(image->cols, image->rows, CV_64F);
//     cv::filter2D(*image, *dest, -1, gaussianKernel, Point(-1, -1), 0, BORDER_CONSTANT);
//
//     printf("----------------------------------------\n");
//     for (int i = 0; i < dest->rows; i++) {
//         for (int j = 0; j < dest->cols; j++) {
//             double d = dest->at<double>(i, j);
//             printf("%3d ", (int) d);
//         }
//         printf("\n----------------------------------------\n");
//     }
//     printf("----------------------------------------\n");
//
//     return dest;
// }

Mat flip(Mat M) {
    Mat result = Mat();
    cv::flip(M, result, -1);
    return result;
}


int main(int argc, char** args) {
    Frame* jpgImage = new Frame("resources/test.jpg");
    Mat img = *(jpgImage->grayscale);

    cv::Mat gaussianKernel = cv::getGaussianKernel(12, 2, CV_64F);
    cv::mulTransposed(gaussianKernel,gaussianKernel,false);
    printf("----------------------------------------\n");
    for (int i = 0; i < gaussianKernel.rows; i++) {
        for (int j = 0; j < gaussianKernel.cols; j++) {
            double d = gaussianKernel.at<double>(i, j);
            printf("%0.5f ", (float) d);
        }
        printf("\n");
    }

    Mat src2 = Mat();
    const int additionalRows = gaussianKernel.rows-1, additionalCols = gaussianKernel.cols-1;
    copyMakeBorder(img,
                   src2,
                   (additionalRows + 1) / 2,
                   (additionalRows) / 2,
                   (additionalCols + 1) / 2,
                   (additionalCols) / 2,
                   BORDER_CONSTANT,
                   Scalar(0));

   Point anchor(gaussianKernel.cols - gaussianKernel.cols/2 - 1, gaussianKernel.rows - gaussianKernel.rows/2 - 1);
   int borderMode = BORDER_CONSTANT;

   Mat dest = Mat();
   filter2D(src2, dest, src2.depth(), flip(gaussianKernel), anchor, 0, borderMode);

   dest = dest.colRange((gaussianKernel.cols-1)/2, dest.cols - gaussianKernel.cols/2)
               .rowRange((gaussianKernel.rows-1)/2, dest.rows - gaussianKernel.rows/2);

   printf("----------------------------------------\n");
   for (int i = 1; i < 20; i++) {
       for (int j = 1; j < 20; j++) {
           uchar d = dest.at<uchar>(i, j);
           printf("%3d ", (int) d);
       }
       printf("\n");
   }

   Mat* destPointer = new Mat(dest);
   printf("----------------------------------------\n");
   for (int i = 1; i < 20; i++) {
       for (int j = 1; j < 20; j++) {
           uchar d = destPointer->at<uchar>(i, j);
           printf("%3d ", (int) d);
       }
       printf("\n");
   }

    // ------------------------------------------------------------------------------------------
    // Frame* jpgImage = new Frame("resources/test.jpg");
    // blur(jpgImage->grayscale, 6);
    // jpgImage->show();

    // ------------------------------------------------------------------------------------------

    // Frame* coloredImage = new Frame("resources/colored-test.jpg");
    // coloredImage->show();
    // ------------------------------------------------------------------------------------------
}
