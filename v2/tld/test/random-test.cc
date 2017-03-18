
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <iostream>

using namespace std;

float ncc(IplImage* im0, IplImage* im1, CvPoint2D32f p0, CvPoint2D32f p1);

int main(int argc, char** argv) {
    IplImage* im0 = cvLoadImage("/Users/mehmetgunturkun/MasterThesis/ceng500-mscthesis/resources/04_pedestrian2/sequence/00001.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    IplImage* im1 = cvLoadImage("/Users/mehmetgunturkun/MasterThesis/ceng500-mscthesis/resources/04_pedestrian2/sequence/00002.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    CvPoint2D32f p0 = CvPoint2D32f();
    p0.x = 100.0f;
    p0.y = 100.0f;

    CvPoint2D32f p1 = CvPoint2D32f();
    p1.x = 100.0f;
    p1.y = 100.0f;

    ncc(im0, im1, p0, p1);

    return 0;

}

float ncc(IplImage* im0, IplImage* im1, CvPoint2D32f p0, CvPoint2D32f p1) {
    IplImage* rec0 = cvCreateImage(cvSize(10, 10), 8, 1);
    IplImage* rec1 = cvCreateImage(cvSize(10, 10), 8, 1);
    IplImage* res = cvCreateImage(cvSize(1, 1), IPL_DEPTH_32F, 1);


    cvGetRectSubPix(im0, rec0, p0);
    cvGetRectSubPix(im1, rec1, p1);

    float xy = 0.0f;
    float xx = 0.0f;
    float yy = 0.0f;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int pix0 = (int) rec0->imageData[i * 10 + j];
            int pix1 = (int) rec1->imageData[i * 10 + j];

            xy += pix0 * pix1;
            xx += pix0 * pix0;
            yy += pix1 * pix1;
        }
    }




    cvMatchTemplate(rec0, rec1, res, CV_TM_CCOEFF_NORMED);

    float nccVal = ((float*) res->imageData)[0];
    float res = xy / sqrt(xx * yy);
    return nccVal;
}
