#include "detector/nn/Patch.hpp"

int Patch::WIDTH = 15;
int Patch::HEIGHT = 15;

Patch::Patch(Mat* d) {
    data = d;
}

Rect* toRect(Frame* frame, Box* box) {
    double left = 0.0;
    if (box->x1 <= 0) {
        left = 0.0;
    } else {
        left = box->x1;
    }

    double right = 0.0;
    if (box->x2 > frame->width) {
        right = frame->width;
    } else {
        right = box->x2;
    }

    double top = 0.0;
    if (box->y1 <= 0) {
        top = 0.0;
    } else {
        top = box->y1;
    }

    double bottom = 0.0;
    if (box->y2 > frame->height) {
        bottom = frame->height;
    } else {
        bottom = box->y2;
    }

    Rect* rect = new Rect(left, top, right - left + 1, bottom - top + 1);
    return rect;
}

Patch::Patch(Frame* frame, Box* box) {
    //TODO Review if we can do with memcpy?
    Rect* rect = toRect(frame, box);
    Mat patch = Mat(*(frame->grayscale), *rect);
    Mat normalizedTmpPatch = Mat(Patch::HEIGHT, Patch::WIDTH, CV_8U, 0.0);
    Mat* normalizedPatch = new Mat(Patch::HEIGHT, Patch::WIDTH, CV_64F, 0.0);
    cv::resize(patch, normalizedTmpPatch, normalizedTmpPatch.size(), INTER_LINEAR);

    Scalar tempVal = cv::mean(normalizedTmpPatch);
    double meanNorm = (double) tempVal[0];
    for (int i = 0 ; i < Patch::HEIGHT; i++) {
        for (int j = 0 ; j < Patch::WIDTH; j++) {
            double diff = (double) normalizedTmpPatch.at<uchar>(i,j) - meanNorm;
            normalizedPatch->at<double>(i,j) =(double) diff;
        }
    }
    data = normalizedPatch;
}
