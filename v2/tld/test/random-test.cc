#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

#include "opencv2/highgui/highgui.hpp"

#include "common/Arguments.hpp"
#include "common/Colors.hpp"

#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    IplImage* image = cvLoadImage("/Users/mehmetgunturkun/MasterThesis/ceng500-mscthesis/resources/04_pedestrian2/sequence/00002.jpg");
    cvShowImage("test", image);
    cvWaitKey(0);

    IplImage* grayImage = cvCreateImage(cvGetSize(image), 8, 1);
    printf("%d - %d\n", image->depth, grayImage->depth);

    cvCvtColor(image, grayImage, CV_BGR2GRAY);

    cvShowImage("grayscale", grayImage);
    cvWaitKey(0);

}
