
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

#define CV_IMAGE_GET_ELEM( image, elemtype, row, col )       \
    CV_IMAGE_ELEM( image, elemtype, row, col )

#define CV_IMAGE_SET_ELEM( image, elemtype, row, col, elem )       \
    elemtype* loc = ((elemtype*)((image)->imageData + (image)->widthStep*(row))); \
    loc[(col)] = elem

bool isCoordinatesInteger(Box* b) {
    double sum = 0.0;
    sum += b->x1;
    sum += b->x2;
    sum += b->y1;
    sum += b->y2;

    double roundedSum = 0.0;
    roundedSum += round(b->x1);
    roundedSum += round(b->x2);
    roundedSum += round(b->y1);
    roundedSum += round(b->y2);

    return sum == roundedSum;
}

IplImage* convertToDoubleWithInts(IplImage* m, Box* b) {
    int height = (int) b->height;
    int width = (int) b->width;

    IplImage* src = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 0.0);

    int x1 = (int) b->x1;
    int y1 = (int) b->y1;
    int x2 = (int) b->x2;
    int y2 = (int) b->y2;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uchar pixel = CV_IMAGE_ELEM(m, uchar, y1 + i, x1 + j);
            double pixel64f = (double) pixel;

            CV_IMAGE_SET_ELEM(src, double, i, j, pixel64f);
            //srcData[i * widthStep + j] = pixel64f;
        }
    }
    return src;
}


IplImage* convertToDoubleWithDoubles(IplImage* m, Box* b) {
    int width = m->width;
    int height = m->height;

    double xmin = -(b->width / 2.0);
    double xmax = (b->width / 2.0);

    double ymin = -(b->height / 2.0);
    double ymax = (b->height / 2.0);

    double* H = (double*) malloc(sizeof(double) * 9);
    H[0] = 1.0;
    H[1] = 0.0;
    H[2] = (b->x1 + b->x2) / 2.0;
    H[3] = 0.0;
    H[4] = 1.0;
    H[5] = (b->y1 + b->y2) / 2.0;
    H[6] = 0.0;
    H[7] = 0.0;
    H[8] = 1.0;

    IplImage* warpedPatch = Warp::warp(m, H, xmin, ymin, xmax, ymax);
    for (int i = 0; i < warpedPatch->height; i++) {
        for (int j = 0; j < warpedPatch->width; j++) {
            double pixel = CV_IMAGE_ELEM(warpedPatch, double, i, j);
            CV_IMAGE_SET_ELEM(warpedPatch, double, i, j, round(pixel));
        }
    }
    return warpedPatch;
}

double computeMean(IplImage* m) {
    int height = m->height;
    int width = m->width;
    int nrOfPixels = height * width;

    double sum = 0.0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pixel = CV_IMAGE_GET_ELEM(m, double, i, j);

            double sum1 = sum + pixel;
            sum = sum1;
        }
    }

    double mean = sum / nrOfPixels;
    return mean;
}

IplImage* createPatch(IplImage* m, Box* b) {
    IplImage* src;
    if (isCoordinatesInteger(b)) {
        src = convertToDoubleWithInts(m, b);
    } else {
        src = convertToDoubleWithDoubles(m, b);
    }

    IplImage* patch = cvCreateImage(cvSize(15, 15), IPL_DEPTH_64F, 0.0);
    cvResize(src, patch, INTER_LINEAR);


    double mean = computeMean(patch);
    for (int i = 0 ; i < 15; i++) {
        for (int j = 0 ; j < 15; j++) {
            double pixel = CV_IMAGE_GET_ELEM(patch, double, i , j);
            double normalizedPixel = pixel - mean;
            CV_IMAGE_SET_ELEM(patch, double, i , j, normalizedPixel);
        }
    }

    return patch;
}

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);
    Sequence sequence = Sequence(args.getString("sequence"));

    Frame* frame = sequence.next();
    Box* box = sequence.initBox;
    Box* shiftedBox = new Box(0, box->x1 + 1.4, box->y1 + 5.9, box->x2 - 5.3, box->y2 -9.121);

    IplImage* grayImage = frame->grayscale;
    IplImage* patch = createPatch(grayImage, box);

    double total = 0.0;
    int height = patch->height;
    int width = patch->width;
    printf("Normalized Patch: %d x %d\n", patch->width, patch->height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pixel = CV_IMAGE_GET_ELEM(patch, double, i, j);
            total += pixel;
            printf("%9.4f ", pixel);
        }
        printf("\n");
    }
}
