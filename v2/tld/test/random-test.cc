
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
    char* str1 = (char*) malloc(sizeof(char) * 3);
    str1[0] = 'a';
    str1[1] = 'a';
    str1[2] = '\0';
    string str2(str1);

    delete[] str1;
}

float ncc(IplImage* im0, IplImage* im1, CvPoint2D32f p0, CvPoint2D32f p1) {
    IplImage* rec0 = cvCreateImage(cvSize(10, 10), 8, 1);
    IplImage* rec1 = cvCreateImage(cvSize(10, 10), 8, 1);
    IplImage* res = cvCreateImage(cvSize(1, 1), IPL_DEPTH_32F, 1);


    cvGetRectSubPix(im0, rec0, p0);
    cvGetRectSubPix(im1, rec1, p1);

    cvMatchTemplate(rec0, rec1, res, CV_TM_CCOEFF_NORMED);

    float nccVal = ((float*) res->imageData)[0];
    printf("NCC: %a\n", nccVal);
    return nccVal;
}
