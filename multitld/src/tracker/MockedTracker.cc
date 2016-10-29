#include "cv.h"

#include "tracker/Tracker.hpp"
#include "common/Colors.hpp"
#include <iostream>
#include <fstream>

#include "common/Logging.hpp"

//********************************
void track(Frame* firstFrame, Frame* secondFrame, Box* box);

class TPoint {
public:
    double x;
    double y;

    TPoint(double x, double y) {
        this->x = x;
        this->y = y;
    }
};

bool isOut(Frame* frame, Box* b) {
    return b->x1 > frame->width || b->y1 > frame->height || b->x2 < 0 || b->y2 < 0;
}

double computeStep(double start, double end, int pointCount) {
    double step = ((end - 5.0) - (start + 5.0)) / (pointCount - 1);
    return step;
}

float distance(CvPoint2D32f p0, CvPoint2D32f p1) {
    float distanceVal = sqrt((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y));
    return distanceVal;
}

float ncc(IplImage* im0, IplImage* im1, CvPoint2D32f p0, CvPoint2D32f p1) {
    IplImage* rec0 = cvCreateImage(cvSize(10, 10), 8, 1);
    IplImage* rec1 = cvCreateImage(cvSize(10, 10), 8, 1);
    IplImage* res = cvCreateImage(cvSize(1, 1), IPL_DEPTH_32F, 1);

    cvGetRectSubPix(im0, rec0, p0);
    cvGetRectSubPix(im1, rec1, p1);

    cvMatchTemplate(rec0, rec1, res, CV_TM_CCOEFF_NORMED);
    float nccVal = ((float*) res->imageData)[0];
    return nccVal;
}



vector<TPoint*> createPoints(Box* box) {
    double x1 = (double) box->x1;
    double x2 = (double) box->x2;

    double y1 = (double) box->y1;
    double y2 = (double) box->y2;

    double horizontalStep = computeStep(x1, x2, 10);
    double verticalStep = computeStep(y1, y2, 10);

    double height = (double) box->height;
    double heightWithPadding = height - 5.0;

    double width = (double) box->width;
    double widthWithPadding = width - 5.0;

    vector<TPoint*> pointList;

    for (double i = 5; i <= widthWithPadding; i = i + horizontalStep) {
        for (double j = 5; j <= heightWithPadding; j = j + verticalStep) {
            double x = x1 + i;
            double y = y1 + j;
            ERROR("Point(%3.4f, %3.4f)", x, y);

            TPoint* point = new TPoint(x, y);
            pointList.push_back(point);
        }
    }

    ERROR("%s", box->toCharArr());
    ERROR("There are %4d points to track", (int) pointList.size());

    return pointList;
}

float median(vector<float> floatVector) {
    int size = (int) floatVector.size();
    if (size == 0) {
        return 0.0f;
    } else if (size == 1) {
        return floatVector[0];
    } else {
        sort(floatVector.begin(), floatVector.end(), [](float a, float b) { return a > b; });
        int middle = size / 2;
        float med = floatVector[middle];
        if (size % 2 == 0) {
            printf("Even\n");
            med = (med + floatVector[middle - 1]) / 2;
        }
        return med;
    }
}

vector<float> pdist(vector<CvPoint2D32f> points, int nrOfPoints) {
    vector<float> dist;
    int nrOfDist = 0;
    for (int i = 0; i < nrOfPoints - 1; i++) {
        CvPoint2D32f p0 = points[i];
        for (int j = i + 1; j < nrOfPoints; j++) {
            nrOfDist += 1;
            CvPoint2D32f p1 = points[j];

            float distVal = distance(p0, p1);
            dist.push_back(distVal);
            printf("%d, P(%3.4f, %3.4f) - P(%3.4f, %3.4f) - %3.4f\n", nrOfDist, p0.x, p0.y, p1.x, p1.y, distVal);
        }
    }
    return dist;
}

void predict(Box* box, int nrOfPoints, vector<CvPoint2D32f> fromPoints, vector<CvPoint2D32f> toPoints) {
    vector<float> dx;
    vector<float> dy;

    for (int i = 0; i < nrOfPoints; i++) {
        CvPoint2D32f fromPoint = fromPoints[i];
        CvPoint2D32f toPoint = toPoints[i];

        dx.push_back(toPoint.x - fromPoint.x);
        dy.push_back(toPoint.y - fromPoint.y);
    }

    float medDx = median(dx);
    float medDy = median(dy);


    vector<float> s0 = pdist(fromPoints, nrOfPoints);
    printf("***************\n");
    vector<float> s1 = pdist(toPoints, nrOfPoints);
    vector<float> s01;
    for (int i = 0; i < (int) s0.size(); i++) {
        s01.push_back(s1[i] / s0[i]);
    }

    printf("Size: %d\n", (int) s01.size());

    float medSx = median(s01);

    float sx = 0.5 * (medSx - 1.0) * box->width;
    float sy = 0.5 * (medSx - 1.0) * box->height;

    printf("dsx: %3.4f, dsy: %3.4f\n", sx, sy);

    printf("Med(dx) = %3.4f, Med(dy) = %3.4f, Med(scale) = %3.4f\n", medDx, medDy, medSx);

    printf("Tracker >>> Box(%3.4f, %3.4f, %3.4f, %3.4f)\n",
        box->x1 - sx + medDx,
        box->y1 - sy + medDy,
        box->x2 + sx + medDx,
        box->y2 + sy + medDy
    );

}

void trackOneFrame(Frame* firstFrame, Frame* secondFrame, Box* box) {
    CvSize imageSize = cvSize(320, 240);

    IplImage* fromImage = new IplImage(*(firstFrame->grayscale));
    IplImage* fromPyramid = cvCreateImage( imageSize, 8, 1 );

    IplImage* toImage = new IplImage(*(secondFrame->grayscale));
    IplImage* toPyramid = cvCreateImage( imageSize, 8, 1 );

    vector<TPoint*> points = createPoints(box);
    int nrOfPoints = (int) points.size();

    CvPoint2D32f* fromPoints = (CvPoint2D32f*)cvAlloc( nrOfPoints * sizeof(CvPoint2D32f));
    CvPoint2D32f* toPoints = (CvPoint2D32f*)cvAlloc( nrOfPoints * sizeof(CvPoint2D32f));
    CvPoint2D32f* bwPoints = (CvPoint2D32f*)cvAlloc( nrOfPoints * sizeof(CvPoint2D32f));

    for (int i = 0; i < nrOfPoints; i++) {
        TPoint* point = points[i];

        fromPoints[i].x = point->x;
        fromPoints[i].y = point->y;

        toPoints[i].x = point->x;
        toPoints[i].y = point->y;

        bwPoints[i].x = point->x;
        bwPoints[i].y = point->y;
    }

    printf("---------------------\n");

    for (int i = 0; i < nrOfPoints; i++) {
        CvPoint2D32f floatPoint = fromPoints[i];
        float x = floatPoint.x;
        float y = floatPoint.y;
        printf("P(%3.4f, %3.4f)\n", x, y);
    }
    char* status = (char*) cvAlloc(nrOfPoints);

    cvCalcOpticalFlowPyrLK(
        fromImage,
        toImage,
        fromPyramid,
        toPyramid,
        fromPoints,
        toPoints,
        nrOfPoints,
        cvSize(4, 4),
        5,
        status,
        0,
        cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03),
        CV_LKFLOW_INITIAL_GUESSES
    );

    printf("---------------------\n");

    for (int i = 0; i < nrOfPoints; i++) {
        CvPoint2D32f floatPoint = toPoints[i];
        float x = floatPoint.x;
        float y = floatPoint.y;
        printf("P(%3.4f, %3.4f)\n", x, y);
    }

    char* status2 = (char*) cvAlloc(nrOfPoints);
    cvCalcOpticalFlowPyrLK(
        toImage,
        fromImage,
        toPyramid,
        fromPyramid,
        toPoints,
        bwPoints,
        nrOfPoints,
        cvSize(4, 4),
        5,
        status2,
        0,
        cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03),
        CV_LKFLOW_INITIAL_GUESSES
    );

    printf("---------------------\n");

    for (int i = 0; i < nrOfPoints; i++) {
        CvPoint2D32f floatPoint = bwPoints[i];
        float x = floatPoint.x;
        float y = floatPoint.y;
        printf("P(%3.4f, %3.4f)\n", x, y);
    }

    vector<float> fbVector;
    vector<float> nccVector;

    for (int i = 0; i < nrOfPoints; i++) {
        CvPoint2D32f fPoint = fromPoints[i];
        CvPoint2D32f tPoint = toPoints[i];
        CvPoint2D32f bPoint = bwPoints[i];

        float fbError = distance(fPoint, bPoint);
        fbVector.push_back(fbError);

        float nccSim = ncc(fromImage, toImage, fPoint, tPoint);
        nccVector.push_back(nccSim);
        printf("P(%3.4f, %3.4f) - P(%3.4f, %3.4f) = %3.4f, %3.4f\n", fPoint.x, fPoint.y, bPoint.x, bPoint.y, fbError, nccSim);
    }

    float medianFbError = median(fbVector);
    float medianNccSim = median(nccVector);
    printf("Med(fb) = %3.4f, Med(ncc) = %3.4f\n", medianFbError, medianNccSim);

    vector<CvPoint2D32f> stableFromPoints;
    vector<CvPoint2D32f> stableToPoints;
    int nrOfStablePoints = 0;
    for (int i = 0; i < nrOfPoints; i++) {
        CvPoint2D32f fPoint = fromPoints[i];
        CvPoint2D32f tPoint = toPoints[i];
        CvPoint2D32f bPoint = bwPoints[i];

        float fbError = distance(fPoint, bPoint);
        float nccSim = ncc(fromImage, toImage, fPoint, tPoint);
        if (fbError <= medianFbError && nccSim >= medianNccSim) {
            printf(GREEN("P(%3.4f, %3.4f) -> P(%3.4f, %3.4f) -> P(%3.4f, %3.4f) = %3.4f, %3.4f\n"),
                fPoint.x, fPoint.y,
                tPoint.x, tPoint.y,
                bPoint.x, bPoint.y, fbError, nccSim);
            nrOfStablePoints += 1;
            stableFromPoints.push_back(fPoint);
            stableToPoints.push_back(tPoint);

        } else {
            printf(RED("P(%3.4f, %3.4f) - P(%3.4f, %3.4f) = %3.4f, %3.4f\n"), fPoint.x, fPoint.y, bPoint.x, bPoint.y, fbError, nccSim);
        }
    }
    predict(box, nrOfStablePoints, stableFromPoints, stableToPoints);
}

//******************************************

vector<Box*> mockedBoxList;

Option<Box>* parseFromLine(string line) {
    StringStream* stream = new StringStream(line, ',');

    double x1 = stod(stream->next());
    double y1 = stod(stream->next());
    double x2 = stod(stream->next());
    double y2 = stod(stream->next());
    double score = stod(stream->next());

    if (isnan(x1) || x1 == 0 || isnan(x2) || isnan(y1) || isnan(y2)) {
        Option<Box>* none = new Option<Box>();
        return none;
    }

    Box* box = new Box(0, x1 - 1, y1 - 1, x2 - 1, y2 - 1);
    box->mockedScore = score;
    Option<Box>* maybeBox = new Option<Box>(box);
    return maybeBox;
}

Tracker::Tracker() {
    // ifstream initFile("/tmp/mockedBoxList.csv");
    ifstream initFile("/tmp/tld/simulations/car/mockedBoxList.csv");
    string line;
    while (getline(initFile, line)) {
        // printf("reading line: %s\n", line.c_str());
        Option<Box>* maybeBox = parseFromLine(line);
        if (maybeBox->isDefined()) {
            mockedBoxList.push_back(maybeBox->get());
        } else {
            mockedBoxList.push_back(nullptr);
        }
    }

    // Load Predefined Box List
}

vector<Box*> Tracker::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    printf("Returning mocked result for %d\n", curr->id);
    if (boxList.size() > 0 && boxList[0] != nullptr) {
        Box* fromBox = boxList[0];
        if (isOut(prev, fromBox)) {
            // do nothing
        } else {
            trackOneFrame(prev, curr, fromBox);
        }
    }

    Box* nextBox = mockedBoxList[curr->id - 1];
    vector<Box*> nextBoxList;
    if (nextBox == nullptr) {
        nextBoxList =  { nullptr };
    } else {
        printf("TRACKER >>> %s, %g\n", nextBox->toCharArr(), nextBox->mockedScore);
        nextBoxList =  { nextBox };
    }
    return nextBoxList;
}
