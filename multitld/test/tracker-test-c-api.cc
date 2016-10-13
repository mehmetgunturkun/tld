#include <stdlib.h>
#include <vector>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"

using namespace std;
using namespace cv;

#define COLOR_BOLD    "\x1b[1m"

#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"

#define COLOR_RESET   "\x1b[0m"


int main(int argc, char** args) {

    vector<Point2f> points;
    points.push_back(Point2f(146.000000, 130.000000));
    points.push_back(Point2f(146.000000, 133.222229));
    points.push_back(Point2f(146.000000, 136.444443));
    points.push_back(Point2f(146.000000, 139.666672));
    points.push_back(Point2f(146.000000, 142.888885));
    points.push_back(Point2f(146.000000, 146.111115));
    points.push_back(Point2f(146.000000, 149.333328));
    points.push_back(Point2f(146.000000, 152.555557));
    points.push_back(Point2f(146.000000, 155.777771));
    points.push_back(Point2f(146.000000, 159.000000));
    points.push_back(Point2f(154.888885, 130.000000));
    points.push_back(Point2f(154.888885, 133.222229));
    points.push_back(Point2f(154.888885, 136.444443));
    points.push_back(Point2f(154.888885, 139.666672));
    points.push_back(Point2f(154.888885, 142.888885));
    points.push_back(Point2f(154.888885, 146.111115));
    points.push_back(Point2f(154.888885, 149.333328));
    points.push_back(Point2f(154.888885, 152.555557));
    points.push_back(Point2f(154.888885, 155.777771));
    points.push_back(Point2f(154.888885, 159.000000));
    points.push_back(Point2f(163.777771, 130.000000));
    points.push_back(Point2f(163.777771, 133.222229));
    points.push_back(Point2f(163.777771, 136.444443));
    points.push_back(Point2f(163.777771, 139.666672));
    points.push_back(Point2f(163.777771, 142.888885));
    points.push_back(Point2f(163.777771, 146.111115));
    points.push_back(Point2f(163.777771, 149.333328));
    points.push_back(Point2f(163.777771, 152.555557));
    points.push_back(Point2f(163.777771, 155.777771));
    points.push_back(Point2f(163.777771, 159.000000));
    points.push_back(Point2f(172.666672, 130.000000));
    points.push_back(Point2f(172.666672, 133.222229));
    points.push_back(Point2f(172.666672, 136.444443));
    points.push_back(Point2f(172.666672, 139.666672));
    points.push_back(Point2f(172.666672, 142.888885));
    points.push_back(Point2f(172.666672, 146.111115));
    points.push_back(Point2f(172.666672, 149.333328));
    points.push_back(Point2f(172.666672, 152.555557));
    points.push_back(Point2f(172.666672, 155.777771));
    points.push_back(Point2f(172.666672, 159.000000));
    points.push_back(Point2f(181.555557, 130.000000));
    points.push_back(Point2f(181.555557, 133.222229));
    points.push_back(Point2f(181.555557, 136.444443));
    points.push_back(Point2f(181.555557, 139.666672));
    points.push_back(Point2f(181.555557, 142.888885));
    points.push_back(Point2f(181.555557, 146.111115));
    points.push_back(Point2f(181.555557, 149.333328));
    points.push_back(Point2f(181.555557, 152.555557));
    points.push_back(Point2f(181.555557, 155.777771));
    points.push_back(Point2f(181.555557, 159.000000));
    points.push_back(Point2f(190.444443, 130.000000));
    points.push_back(Point2f(190.444443, 133.222229));
    points.push_back(Point2f(190.444443, 136.444443));
    points.push_back(Point2f(190.444443, 139.666672));
    points.push_back(Point2f(190.444443, 142.888885));
    points.push_back(Point2f(190.444443, 146.111115));
    points.push_back(Point2f(190.444443, 149.333328));
    points.push_back(Point2f(190.444443, 152.555557));
    points.push_back(Point2f(190.444443, 155.777771));
    points.push_back(Point2f(190.444443, 159.000000));
    points.push_back(Point2f(199.333328, 130.000000));
    points.push_back(Point2f(199.333328, 133.222229));
    points.push_back(Point2f(199.333328, 136.444443));
    points.push_back(Point2f(199.333328, 139.666672));
    points.push_back(Point2f(199.333328, 142.888885));
    points.push_back(Point2f(199.333328, 146.111115));
    points.push_back(Point2f(199.333328, 149.333328));
    points.push_back(Point2f(199.333328, 152.555557));
    points.push_back(Point2f(199.333328, 155.777771));
    points.push_back(Point2f(199.333328, 159.000000));
    points.push_back(Point2f(208.222229, 130.000000));
    points.push_back(Point2f(208.222229, 133.222229));
    points.push_back(Point2f(208.222229, 136.444443));
    points.push_back(Point2f(208.222229, 139.666672));
    points.push_back(Point2f(208.222229, 142.888885));
    points.push_back(Point2f(208.222229, 146.111115));
    points.push_back(Point2f(208.222229, 149.333328));
    points.push_back(Point2f(208.222229, 152.555557));
    points.push_back(Point2f(208.222229, 155.777771));
    points.push_back(Point2f(208.222229, 159.000000));
    points.push_back(Point2f(217.111115, 130.000000));
    points.push_back(Point2f(217.111115, 133.222229));
    points.push_back(Point2f(217.111115, 136.444443));
    points.push_back(Point2f(217.111115, 139.666672));
    points.push_back(Point2f(217.111115, 142.888885));
    points.push_back(Point2f(217.111115, 146.111115));
    points.push_back(Point2f(217.111115, 149.333328));
    points.push_back(Point2f(217.111115, 152.555557));
    points.push_back(Point2f(217.111115, 155.777771));
    points.push_back(Point2f(217.111115, 159.000000));
    points.push_back(Point2f(226.000000, 130.000000));
    points.push_back(Point2f(226.000000, 133.222229));
    points.push_back(Point2f(226.000000, 136.444443));
    points.push_back(Point2f(226.000000, 139.666672));
    points.push_back(Point2f(226.000000, 142.888885));
    points.push_back(Point2f(226.000000, 146.111115));
    points.push_back(Point2f(226.000000, 149.333328));
    points.push_back(Point2f(226.000000, 152.555557));
    points.push_back(Point2f(226.000000, 155.777771));
    points.push_back(Point2f(226.000000, 159.000000));

    int nPts = (int) points.size();
    int win_size = 4;

    char* status = (char*)  cvAlloc(nPts);

    CvSize imageSize = cvSize(320, 240);

    IplImage* firstFrame = cvLoadImage("/Users/mehmetgunturkun/MasterThesis/resources/tld/car/sequence/00001.jpg", CV_LOAD_IMAGE_COLOR);
    IplImage* firstFrameGray = cvCreateImage( imageSize, 8, 1 );
    // cvCvtColor(firstFrameGray, firstFrame, CV_BGR2GRAY);
    IplImage* firstPyramid = cvCreateImage( imageSize, 8, 1 );

    IplImage* secondFrame = cvLoadImage("/Users/mehmetgunturkun/MasterThesis/resources/tld/car/sequence/00002.jpg", CV_LOAD_IMAGE_COLOR);
    IplImage* secondFrameGray = cvCreateImage( imageSize, 8, 1 );
    // IplImage* secondFrameGray = cvCvtColor(firstFrame, CV_LOAD_IMAGE_GRAYSCALE);
    IplImage* secondPyramid = cvCreateImage( imageSize, 8, 1 );

    CvPoint2D32f* fromPoints = (CvPoint2D32f*)cvAlloc(nPts*sizeof(CvPoint2D32f));
    CvPoint2D32f* toPoints = (CvPoint2D32f*)cvAlloc(nPts*sizeof(CvPoint2D32f));

    int Level = 5;

    for (int i = 0; i < nPts; i++) {
        Point2f p = points[i];
        fromPoints[i].x = p.x;
        fromPoints[i].y = p.y;

        toPoints[i].x = p.x;
        toPoints[i].y = p.y;
    }

    cvCalcOpticalFlowPyrLK(
        firstFrameGray,
        secondFrameGray,
        firstPyramid,
        secondPyramid,
        fromPoints,
        toPoints,
        nPts,
        cvSize(win_size, win_size),
        Level,
        status,
        0,
        cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20,0.03),
        CV_LKFLOW_INITIAL_GUESSES);

    printf("mc2000\n");

    for (int i = 0; i < nPts; i++) {
        int state = (int) status[i];
        CvPoint2D32f p0 = fromPoints[i];
        CvPoint2D32f p1 = toPoints[i];
        if (state == 1) {
            printf(COLOR_GREEN "P0(%g, %g) >> P1(%g, %g)\n" COLOR_RESET, p0.x, p0.y, p1.x, p1.y);
        } else {
            printf(COLOR_RED "P0(%g, %g) >> P1(%g, %g)\n" COLOR_RESET, p0.x, p0.y, p1.x, p1.y);
        }
    }
    return 0;
}
