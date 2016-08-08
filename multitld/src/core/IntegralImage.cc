#include "core/IntegralImage.hpp"
MeanVariance::MeanVariance(float m, float v) {
        mean = m;
        variance = v;
}

IntegralImage::IntegralImage(Mat* img) {
    step = img->cols;
    width = img->cols;
    height = img->rows;
    integralImage = computeIntegralImage(img);
    integralSquaredImage = computeSquaredIntegralImage(img);
}

int* IntegralImage::computeIntegralImage(Mat* img) {
    return computeIntegralImage(img, [](int pixel) {return pixel;});
}

int* IntegralImage::computeSquaredIntegralImage(Mat* img) {
    return computeIntegralImage(img, [](int pixel) {return pixel * pixel;});
}


int* IntegralImage::computeIntegralImage(Mat* img, int (f)(int)) {
    int* integral = (int*) malloc(sizeof(int) * width * height);
    for ( int i = 0; i < height; i++) {
            for( int j = 0; j < width; j++) {
                    int left = 0;
                    int up = 0;
                    int left_up = 0;

                    if (i > 0) {
                            up = integral[((i-1)*width) + j];
                    }
                    if ( j > 0) {
                            left = integral[(i*width) +j-1];
                    }

                    if ( i > 0 && j > 0) {
                            left_up = integral[(i-1)*width + j-1];
                    }
                    int sum = up + left - left_up + f(img->data[i*width+j]);
                    integral[(i*width) + j] = sum;
            }
    }
    return integral;
}

MeanVariance* IntegralImage::computeMeanVariance(int x, int y, int w, int h) {
    int windowSum = computeSubWindow(y, x, w, h, false);
    int squaredWindowSum  = computeSubWindow(y, x, w, h, true);

    int area = w * h;
    float mean = (float) windowSum / area;
    float squaredMean = (float) squaredWindowSum / area;
    float variance = (float) squaredMean - (mean*mean);

    if (windowSum < 0 || squaredWindowSum < 0) {
        printf("X: %d, Y: %d, W: %d, H: %d, SUM: %d, SQUARED-SUM: %d, AREA: %d, VAR: %f\n",x, y, w, h, windowSum, squaredWindowSum, area, variance);
    }

    MeanVariance* meanVariance = new MeanVariance(mean, variance);
    return meanVariance;
}

int IntegralImage::computeSubWindow(int row, int col, int pWidth, int pHeight, bool isSquared) {
        int* data = NULL;
        if (!isSquared) {
                data = integralImage;
        } else {
                data = integralSquaredImage;
        }

        int topLeft = 0;
        int bottomLeft = 0;
        int topRight = 0;
        int bottomRight = 0;

        if (col > 0) {
                bottomLeft = data[(row+pHeight-1)*width + col-1];
        }

        if (row > 0) {
                topRight = data[(row-1)*width + col + pWidth-1];
        }

        if (row > 0 && col > 0) {
                topLeft = data[(row-1)*width + col-1];
        }

        bottomRight = data[(row+pHeight-1)*width + col + pWidth-1];
        int sum = bottomRight + topLeft - (topRight + bottomLeft);
        return sum;
}
