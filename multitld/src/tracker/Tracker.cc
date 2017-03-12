#include <iostream>
#include <fstream>

#include "cv.h"

#include "tracker/Tracker.hpp"
#include "common/Colors.hpp"

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

void loadMockedBoxList() {
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
}

Tracker::Tracker() {
    this->MARGIN = 5;
    this->TERM_CRITERIA = new TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
    this->WIN_SIZE = new Size(4, 4);
    this->fbErrorThreshold = 15.0f;

    loadMockedBoxList();
    printf("Tracker is created\n");
}

vector<Box*> getStableBoxes(int nrOfBoxes, vector<Box*> boxList) {
    vector<Box*> stableBoxList;
    for (int i = 0; i < nrOfBoxes; i++) {
        Box* box = boxList[i];
        if (box != nullptr) {
            stableBoxList.push_back(box);
        }
    }
    return stableBoxList;
}

vector<Box*> trackByMock(Frame* prev, Frame* curr, vector<Box*> boxList) {
    Box* nextBox = mockedBoxList[curr->id - 1];
    vector<Box*> nextBoxList;

    if (nextBox == nullptr) {
        nextBoxList =  { nullptr };
    } else {
        printf("MOCK-TRACKER >>> %s, %g\n", nextBox->toCharArr(), nextBox->mockedScore);
        nextBoxList =  { nextBox };
    }
    return nextBoxList;
}

vector<Box*> Tracker::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    vector<Box*> nextBoxList = trackSilently(prev, curr, boxList);
    return nextBoxList;
}

vector<Box*> Tracker::trackSilently(Frame* prev, Frame* curr, vector<Box*> boxList) {
    int nrOfBoxes = boxList.size();

    vector<Box*> stableBoxList = getStableBoxes(nrOfBoxes, boxList);
    int nrOfStableBoxes = (int) stableBoxList.size();

    if (nrOfStableBoxes == 0) {
        return boxList;
    } else {
        vector<tld::Point*> points = decomposePoints(stableBoxList, nrOfStableBoxes);
        int nrOfPoints = (int) points.size();

        vector<FBPoint*> trackedPoints = track(prev, curr, nrOfPoints, points);
        vector<Box*> estimatedBoxList = fragmentAndEstimateBoxList(
            prev,
            curr,
            nrOfStableBoxes,
            stableBoxList,
            nrOfBoxes,
            boxList,
            trackedPoints
        );
        return estimatedBoxList;
    }
}

float Tracker::computeStep(float start, float end, int pointCount) {
    return ((end - MARGIN) - (start + MARGIN)) / (pointCount - 1);
}

vector<tld::Point*> Tracker::decomposePoints(vector<Box*> boxList, int nrOfBoxes) {
    vector<tld::Point*> points;
    int id = 0;
    for (int i = 0; i < nrOfBoxes; i++) {
        Box* box = boxList[i];
        int nrOfPoints = 0;
        float horizontalStep = computeStep(box->x1, box->x2, 10);
        float verticalStep = computeStep(box->y1, box->y2, 10);
        for (float j = MARGIN; j <= box->height - MARGIN; j = j + verticalStep) {
            for (float i = MARGIN; i <= box->width - MARGIN; i = i + horizontalStep) {
                // Point2f point2f = Point2f(, box->y1 + j);

                CvPoint2D32f* point32f = (CvPoint2D32f*) cvAlloc(sizeof(CvPoint2D32f));
                point32f->x = box->x1 + i;
                point32f->y = box->y1 + j;

                tld::Point* point = new tld::Point(point32f);
                DEBUG("P(%3.4f, %3.4f)", point->underlying->x, point->underlying->y);

                nrOfPoints += 1;
                points.push_back(point);
                id++;
            }
        }
        box->nrOfPoints = nrOfPoints;
    }
    return points;
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

vector<FBPoint*> Tracker::track(Frame* prev, Frame* curr, int nrOfPoints, vector<tld::Point*> points) {
    CvSize imageSize = cvSize(prev->width, prev->height);

    IplImage* fromImage = new IplImage(*(prev->grayscale));
    IplImage* fromPyramid = cvCreateImage( imageSize, 8, 1 );

    IplImage* toImage = new IplImage(*(curr->grayscale));
    IplImage* toPyramid = cvCreateImage( imageSize, 8, 1 );

    CvPoint2D32f* fromPoints = (CvPoint2D32f*)cvAlloc(nrOfPoints * sizeof(CvPoint2D32f));
    CvPoint2D32f* toPoints = (CvPoint2D32f*)cvAlloc(nrOfPoints * sizeof(CvPoint2D32f));
    CvPoint2D32f* bwPoints = (CvPoint2D32f*)cvAlloc(nrOfPoints * sizeof(CvPoint2D32f));

    char* statusForw = (char*)  cvAlloc(nrOfPoints);
    char* statusBack = (char*)  cvAlloc(nrOfPoints);

    for (int i = 0; i < nrOfPoints; i++) {
        tld::Point* point = points[i];

        CvPoint2D32f* fPoint = fromPoints + i;
        CvPoint2D32f* tPoint = toPoints + i;
        CvPoint2D32f* bwPoint = bwPoints + i;

        fPoint->x = point->underlying->x;
        fPoint->y = point->underlying->y;

        tPoint->x = point->underlying->x;
        tPoint->y = point->underlying->y;

        bwPoint->x = point->underlying->x;
        bwPoint->y = point->underlying->y;
    }

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
        statusForw,
        0,
        cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20,0.03),
        CV_LKFLOW_INITIAL_GUESSES
    );

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
        statusBack,
        0,
        cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20,0.03),
        CV_LKFLOW_INITIAL_GUESSES | CV_LKFLOW_PYR_A_READY | CV_LKFLOW_PYR_B_READY
    );

    vector<FBPoint*> fbPoints;
    for (int i = 0; i < nrOfPoints; i++) {
        int state = (int) statusForw[i];
        if (state == 1) {
            CvPoint2D32f* fPoint = fromPoints + i;
            tld::Point* fromPoint = new tld::Point(fPoint);

            CvPoint2D32f* tPoint = toPoints + i;
            tld::Point* toPoint = new tld::Point(tPoint);

            CvPoint2D32f* bPoint = bwPoints + i;
            tld::Point* bwPoint = new tld::Point(bPoint);

            float fbError = distance(*fPoint, *bPoint);
            float nccSim = ncc(fromImage, toImage, *fPoint, *tPoint);

            println("P(%4.3f, %4.3f) -> P(%4.3f, %4.3f) -> P(%4.3f, %4.3f)",
                fPoint->x, fPoint->y,
                tPoint->x, tPoint->y,
                bPoint->x, bPoint->y
            );


            FBPoint* fbPoint = new FBPoint(fromPoint, toPoint, bwPoint, fbError, nccSim);
            fbPoints.push_back(fbPoint);
        } else {
            fbPoints.push_back(FBPoint::failed);
        }
    }

    return fbPoints;
}

vector<tld::Point*> Tracker::lkTrack(Frame* prev, Frame* curr, vector<tld::Point*> srcPoints) {
    CvSize imageSize = cvSize(prev->width, prev->height);

    IplImage* fromImage = new IplImage(*(prev->grayscale));
    IplImage* fromPyramid = cvCreateImage( imageSize, 8, 1 );

    IplImage* toImage = new IplImage(*(curr->grayscale));
    IplImage* toPyramid = cvCreateImage( imageSize, 8, 1 );

    int pointCount = (int) srcPoints.size();
    CvPoint2D32f* fromPoints = (CvPoint2D32f*)cvAlloc(pointCount*sizeof(CvPoint2D32f));
    CvPoint2D32f* toPoints = (CvPoint2D32f*)cvAlloc(pointCount*sizeof(CvPoint2D32f));

    int nrOfPoints = 0;
    for (int i = 0; i < pointCount; i++) {
        tld::Point* point = srcPoints[i];
        if (point->state) {
            fromPoints[nrOfPoints].x = point->underlying->x;
            fromPoints[nrOfPoints].y = point->underlying->y;

            toPoints[nrOfPoints].x = point->underlying->x;
            toPoints[nrOfPoints].y = point->underlying->y;

            nrOfPoints += 1;
        }
    }

    char* status = (char*)  cvAlloc(nrOfPoints);

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
        cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20,0.03),
        CV_LKFLOW_INITIAL_GUESSES);

    vector<tld::Point*> targetPoints;
    int id = 0;
    for (int i = 0; i < pointCount; i++) {
        tld::Point* srcPoint = srcPoints[i];
        tld::Point* targetPoint;
        if (srcPoint->state) {
            CvPoint2D32f* toPoint2f = toPoints + id;
            // Point2f toPoint = Point2f(toPoint2f->x, toPoint2f.y);
            uchar state = status[id];
            if (state) {
                targetPoint = new tld::Point(toPoint2f);
            } else {
                targetPoint = tld::Point::failed;
            }
            id += 1;
        } else {
            targetPoint = tld::Point::failed;
        }
        targetPoints.push_back(targetPoint);
    }

    return targetPoints;
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
            med = (med + floatVector[middle - 1]) / 2;
        }
        return med;
    }
}

vector<float> pdist(vector<CvPoint2D32f*> points, int nrOfPoints) {
    vector<float> dist;
    int nrOfDist = 0;
    for (int i = 0; i < nrOfPoints - 1; i++) {
        CvPoint2D32f* p0 = points[i];
        for (int j = i + 1; j < nrOfPoints; j++) {
            nrOfDist += 1;
            CvPoint2D32f* p1 = points[j];

            float distVal = distance(*p0, *p1);
            dist.push_back(distVal);
            DEBUG("%d, P(%3.4f, %3.4f) - P(%3.4f, %3.4f) - %3.4f\n", nrOfDist, p0->x, p0->y, p1->x, p1->y, distVal);
        }
    }
    return dist;
}

bool isOut(Frame* frame, Box* b) {
    return b->x1 > frame->width || b->y1 > frame->height || b->x2 < 0 || b->y2 < 0;
}

Option<Box>* Tracker::estimate(Frame* prev, Frame* curr, Box* box, vector<FBPoint*> trackedPoints, int start, int end) {
    vector<float> fbErrors;
    vector<float> nccErrors;

    println(YELLOW("REAL-TRACKER(IN) >> %s"), box->toCharArr());

    int nrOfStablePoints = 0;
    for (int i = start; i < end; i++) {
        FBPoint* fbPoint = trackedPoints[i];
        if (fbPoint->state) {
            nrOfStablePoints += 1;

            fbErrors.push_back(fbPoint->fbError);
            nccErrors.push_back(fbPoint->ncc);
        }
    }

    if (nrOfStablePoints == 0) {
        printf("Invalid Box -- No Stable Point - %d\n", nrOfStablePoints);
        Option<Box>* failedBox = new Option<Box>();
        return failedBox;
    }

    float medFBE = median(fbErrors);
    float medNCC = median(nccErrors);

    if (medFBE > fbErrorThreshold) {
        printf("Invalid Box -- high fb error - %f\n", medFBE);
        Option<Box>* failedBox = new Option<Box>();
        return failedBox;
    }

    vector<FBPoint*> reliablePoints;
    for (int i = start; i < end; i++) {
        FBPoint* fbPoint = trackedPoints[i];
        if (fbPoint->state && fbPoint->fbError <= medFBE && fbPoint->ncc >= medNCC) {
            reliablePoints.push_back(fbPoint);
        }
    }

    int nrOfReliablePoints = reliablePoints.size();
    if (nrOfReliablePoints == 0) {
        printf("No reliable boxes\n");
        Option<Box>* failedBox = new Option<Box>();
        return failedBox;
    }

    vector<CvPoint2D32f*> fromPoints;
    vector<CvPoint2D32f*> toPoints;

    vector<float> dxList(nrOfReliablePoints);
    vector<float> dyList(nrOfReliablePoints);
    for (int i = 0; i < nrOfReliablePoints; i++) {
        FBPoint* fbPoint = reliablePoints[i];

        tld::Point* srcPoint = fbPoint->src;
        tld::Point* targetPoint = fbPoint->to;

        fromPoints.push_back(srcPoint->underlying);
        toPoints.push_back(targetPoint->underlying);

        dxList[i] = fbPoint->dx;
        dyList[i] = fbPoint->dy;
    }

    vector<float> s0 = pdist(fromPoints, nrOfReliablePoints);
    vector<float> s1 = pdist(toPoints, nrOfReliablePoints);
    vector<float> s01;
    for (int i = 0; i < (int) s0.size(); i++) {
        s01.push_back(s1[i] / s0[i]);
    }

    float medS = median(s01);
    float sx = 0.5 * (medS - 1.0) * box->width;
    float sy = 0.5 * (medS - 1.0) * box->height;

    float medX = median(dxList);
    float medY = median(dyList);

    Box* movedBox = box->move(medX, sx, medY, sy);
    if (isOut(curr, movedBox)) {
        println(RED("REAL-TRACKER(OUT) >> %s"), movedBox->toCharArr());
        Option<Box>* failedBox = new Option<Box>();
        return failedBox;
    } else {
        println(GREEN("REAL-TRACKER(OUT) >> %s"), movedBox->toCharArr());
        Option<Box>* successBox = new Option<Box>(movedBox);
        return successBox;
    }

}

vector<Box*> Tracker::fragmentAndEstimateBoxList(Frame* prev,
                                                 Frame* curr,
                                                 int nrOfStableBoxes,
                                                 vector<Box*> stableBoxList,
                                                 int nrOfBoxes,
                                                 vector<Box*> boxList,
                                                 vector<FBPoint*> trackedPoints) {
    vector<Box*> estimatedBoxList;
    int i = 0;
    int j = 0;

    int cursor = 0;
    while (i < nrOfBoxes) {
        Box* box = boxList[i];
        if (box == nullptr) {
            estimatedBoxList.push_back(nullptr);
        } else {
            Box* stableBox = stableBoxList[j];
            int nrOfPoints = stableBox->nrOfPoints;
            Option<Box>* maybeBox = estimate(prev, curr, stableBox, trackedPoints, cursor, cursor + nrOfPoints);
            if (maybeBox->isDefined()) {
                estimatedBoxList.push_back(maybeBox->get());
            } else {
                estimatedBoxList.push_back(nullptr);
            }
            cursor += nrOfPoints;
            j += 1;
        }
        i += 1;
    }
    return estimatedBoxList;
}
