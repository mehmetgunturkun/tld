#include "common/Colors.hpp"
#include "tracker/Tracker.hpp"


class SamplePoint {
public:
    double x;
    double y;
    SamplePoint(double x, double y) {
        this->x = x;
        this->y = y;
    }
};

int main(int argc, char** args) {

    vector<SamplePoint*> points = {
        new SamplePoint(146, 130),
        new SamplePoint(154.889, 130),
        new SamplePoint(163.778, 130),
        new SamplePoint(172.667, 130),
        new SamplePoint(181.556, 130),
        new SamplePoint(190.444, 130),
        new SamplePoint(199.333, 130),
        new SamplePoint(208.222, 130),
        new SamplePoint(217.111, 130),
        new SamplePoint(226, 130),
        new SamplePoint(146, 132.222),
        new SamplePoint(154.889, 132.222),
        new SamplePoint(163.778, 132.222),
        new SamplePoint(172.667, 132.222),
        new SamplePoint(181.556, 132.222),
        new SamplePoint(190.444, 132.222),
        new SamplePoint(199.333, 132.222),
        new SamplePoint(208.222, 132.222),
        new SamplePoint(217.111, 132.222),
        new SamplePoint(226, 132.222),
        new SamplePoint(146, 135.444),
        new SamplePoint(154.889, 135.444),
        new SamplePoint(163.778, 135.444),
        new SamplePoint(172.667, 135.444),
        new SamplePoint(181.556, 135.444),
        new SamplePoint(190.444, 135.444),
        new SamplePoint(199.333, 135.444),
        new SamplePoint(208.222, 135.444),
        new SamplePoint(217.111, 135.444),
        new SamplePoint(226, 135.444),
        new SamplePoint(146, 138.667),
        new SamplePoint(154.889, 138.667),
        new SamplePoint(163.778, 138.667),
        new SamplePoint(172.667, 138.667),
        new SamplePoint(181.556, 138.667),
        new SamplePoint(190.444, 138.667),
        new SamplePoint(199.333, 138.667),
        new SamplePoint(208.222, 138.667),
        new SamplePoint(217.111, 138.667),
        new SamplePoint(226, 138.667),
        new SamplePoint(146, 141.889),
        new SamplePoint(154.889, 141.889),
        new SamplePoint(163.778, 141.889),
        new SamplePoint(172.667, 141.889),
        new SamplePoint(181.556, 141.889),
        new SamplePoint(190.444, 141.889),
        new SamplePoint(199.333, 141.889),
        new SamplePoint(208.222, 141.889),
        new SamplePoint(217.111, 141.889),
        new SamplePoint(226, 141.889),
        new SamplePoint(146, 145.111),
        new SamplePoint(154.889, 145.111),
        new SamplePoint(163.778, 145.111),
        new SamplePoint(172.667, 145.111),
        new SamplePoint(181.556, 145.111),
        new SamplePoint(190.444, 145.111),
        new SamplePoint(199.333, 145.111),
        new SamplePoint(208.222, 145.111),
        new SamplePoint(217.111, 145.111),
        new SamplePoint(226, 145.111),
        new SamplePoint(146, 148.333),
        new SamplePoint(154.889, 148.333),
        new SamplePoint(163.778, 148.333),
        new SamplePoint(172.667, 148.333),
        new SamplePoint(181.556, 148.333),
        new SamplePoint(190.444, 148.333),
        new SamplePoint(199.333, 148.333),
        new SamplePoint(208.222, 148.333),
        new SamplePoint(217.111, 148.333),
        new SamplePoint(226, 148.333),
        new SamplePoint(146, 151.556),
        new SamplePoint(154.889, 151.556),
        new SamplePoint(163.778, 151.556),
        new SamplePoint(172.667, 151.556),
        new SamplePoint(181.556, 151.556),
        new SamplePoint(190.444, 151.556),
        new SamplePoint(199.333, 151.556),
        new SamplePoint(208.222, 151.556),
        new SamplePoint(217.111, 151.556),
        new SamplePoint(226, 151.556),
        new SamplePoint(146, 154.778),
        new SamplePoint(154.889, 154.778),
        new SamplePoint(163.778, 154.778),
        new SamplePoint(172.667, 154.778),
        new SamplePoint(181.556, 154.778),
        new SamplePoint(190.444, 154.778),
        new SamplePoint(199.333, 154.778),
        new SamplePoint(208.222, 154.778),
        new SamplePoint(217.111, 154.778),
        new SamplePoint(226, 154.778),
        new SamplePoint(146, 158),
        new SamplePoint(154.889, 158),
        new SamplePoint(163.778, 158),
        new SamplePoint(172.667, 158),
        new SamplePoint(181.556, 158),
        new SamplePoint(190.444, 158),
        new SamplePoint(199.333, 158),
        new SamplePoint(208.222, 158),
        new SamplePoint(217.111, 158),
        new SamplePoint(226, 158)
    };

    int nPts = (int) points.size();
    int win_size = 4;

    char* status = (char*)  cvAlloc(nPts);

    CvSize imageSize = cvSize(320, 240);

    IplImage* firstFrame = cvLoadImage("/Users/mehmetgunturkun/MasterThesis/resources/tld/car/sequence/00001.jpg", CV_LOAD_IMAGE_COLOR);
    IplImage* firstPyramid = cvCreateImage( imageSize, 8, 1 );

    IplImage* secondFrame = cvLoadImage("/Users/mehmetgunturkun/MasterThesis/resources/tld/car/sequence/00002.jpg", CV_LOAD_IMAGE_COLOR);
    IplImage* secondPyramid = cvCreateImage( imageSize, 8, 1 );

    CvPoint2D32f* fromPoints = (CvPoint2D32f*)cvAlloc(nPts*sizeof(CvPoint2D32f));
    CvPoint2D32f* toPoints = (CvPoint2D32f*)cvAlloc(nPts*sizeof(CvPoint2D32f));

    int Level = 5;

    for (int i = 0; i < nPts; i++) {
        SamplePoint* p = points[i];
        fromPoints[i].x = p->x;    
        fromPoints[i].y = p->y;

        toPoints[i].x = p->x;
        toPoints[i].y = p->y;
    }

    cvCalcOpticalFlowPyrLK(
        firstFrame,
        secondFrame,
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
