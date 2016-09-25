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
        new SamplePoint(147, 131),
        new SamplePoint(154.889, 130),
        new SamplePoint(163.778, 130),
        new SamplePoint(172.667, 130),
        new SamplePoint(181.556, 130),
        new SamplePoint(190.444, 130),
        new SamplePoint(199.333, 130),
        new SamplePoint(208.222, 130),
        new SamplePoint(217.111, 130),
        new SamplePoint(226, 130),
        new SamplePoint(146, 133.222)
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
        cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03),
        CV_LKFLOW_INITIAL_GUESSES);

    printf("mc2000\n");

    for (int i = 0; i < nPts; i++) {
        CvPoint2D32f p = toPoints[i];
        printf("P(%g, %g)\n", p.x, p.y);
    }
    return 0;
}
