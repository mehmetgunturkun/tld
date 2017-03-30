#include "detector/nn/Patch.hpp"

bool isCoordinatesInteger(Box* b);
IplImage* convertToDoubleWithInts(IplImage* m, Box* b);
IplImage* convertToDoubleWithDoubles(IplImage* m, Box* b);
double computeMean(IplImage* m);

int Patch::WIDTH = 15;

int Patch::HEIGHT = 15;

Patch::Patch(Frame* frame, Box* box) {
    IplImage* m = frame->grayscale;

    IplImage* src;
    if (isCoordinatesInteger(box)) {
        src = convertToDoubleWithInts(m, box);
    } else {
        src = convertToDoubleWithDoubles(m, box);
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

    data = patch;
}

string Patch::toString() {
    stringstream ss;
    ss  << "--------------------------------------------------\n";
    for (int i = 0 ; i < 15; i++) {
        for (int j = 0 ; j < 15; j++) {
            double pixel = CV_IMAGE_GET_ELEM(data, double, i , j);
            ss  << pixel << " ";
        }
        ss << endl;
    }
    ss  << "--------------------------------------------------\n";

    return ss.str();
}


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

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uchar pixel = CV_IMAGE_ELEM(m, uchar, y1 + i, x1 + j);
            double pixel64f = (double) pixel;

            CV_IMAGE_SET_ELEM(src, double, i, j, pixel64f);
        }
    }
    return src;
}


IplImage* convertToDoubleWithDoubles(IplImage* m, Box* b) {
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

double Patch::computeVariance(Patch* patch) {
    IplImage* patchData = patch->data;
    double sum = 0.0;
    for (int i = 0 ; i < Patch::HEIGHT; i++) {
        for (int j = 0 ; j < Patch::WIDTH; j++) {
            double p = CV_IMAGE_GET_ELEM(patchData, double, i, j);
            sum += p;
        }
    }

    double mean = sum / 225;

    double variance = 0.0;
    for (int i = 0 ; i < Patch::HEIGHT; i++) {
        for (int j = 0 ; j < Patch::WIDTH; j++) {
            double p = CV_IMAGE_GET_ELEM(patchData, double, i, j);
            p = p - mean;
            variance += (p * p);
        }
    }

    variance = variance / 224;
    return variance;
}
