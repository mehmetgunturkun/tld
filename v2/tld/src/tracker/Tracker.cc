#include "tracker/Tracker.hpp"

Tracker::Tracker() {
    this->margin = 5;
    this->termCriteria = new TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);;
    this->winSize = new Size(4, 4);

    this->fbErrorThreshold = 15.0f;
}

Tracker::~Tracker() {

}

vector<Box*> Tracker::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    // filter stable boxes
    int nrOfBoxes = (int) boxList.size();
    vector<Box*> stableBoxList = getStableBoxes(nrOfBoxes, boxList);
    int nrOfStableBoxes = (int) stableBoxList.size();

    if (nrOfStableBoxes == 0) {
        return boxList;
    } else {
        // generate points
        vector<CvPoint2D32f*> points = decomposePoints(nrOfStableBoxes, stableBoxList);
        int nrOfPoints = (int) points.size();

        // track each point with lucas-kanade
        vector<FBPoint*> trackedPoints = track(prev, curr, nrOfPoints, points);

        // estimate next box
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

vector<Box*> Tracker::getStableBoxes(int nrOfBoxes, vector<Box*> boxList) {
    vector<Box*> stableBoxList;
    for (int i = 0; i < nrOfBoxes; i++) {
        Box* box = boxList[i];
        if (box != nullptr) {
            stableBoxList.push_back(box);
        }
    }
    return stableBoxList;
}

vector<CvPoint2D32f*> Tracker::decomposePoints(int nrOfBoxes, vector<Box*> boxList) {
    vector<CvPoint2D32f*> points;
    int id = 0;
    for (int i = 0; i < nrOfBoxes; i++) {
        Box* box = boxList[i];
        int nrOfPoints = 0;
        float horizontalStep = computeStep(box->x1, box->x2, 10);
        float verticalStep = computeStep(box->y1, box->y2, 10);
        for (float j = margin; j <= box->height - margin; j = j + verticalStep) {
            for (float i = margin; i <= box->width - margin; i = i + horizontalStep) {
                CvPoint2D32f* point = (CvPoint2D32f*) cvAlloc(sizeof(CvPoint2D32f));
                point->x = box->x1 + i;
                point->y = box->y1 + j;

                nrOfPoints += 1;
                points.push_back(point);
                id++;
            }
        }
        box->nrOfPoints = nrOfPoints;
    }
    return points;
}

float Tracker::computeStep(float start, float end, int pointCount) {
    return ((end - margin) - (start + margin)) / (pointCount - 1);
}

float distance(CvPoint2D32f p0, CvPoint2D32f p1) {
    float distanceVal = sqrt((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y));
    return distanceVal;
}

float ncc(IplImage* im0, IplImage* im1, CvPoint2D32f p0, CvPoint2D32f p1) {
    IplImage* rec0 = cvCreateImage(cvSize(10, 10), 8, 1);
    IplImage* rec1 = cvCreateImage(cvSize(10, 10), 8, 1);
    IplImage* res = cvCreateImage(cvSize(1, 1), IPL_DEPTH_32F, 1);

    printf("P0(%a, %a) -> P(%a, %a)\n", p0.x, p0.y, p1.x, p1.y);

    cvGetRectSubPix(im0, rec0, p0);
    cvGetRectSubPix(im1, rec1, p1);

    cvMatchTemplate(rec0, rec1, res, CV_TM_CCOEFF_NORMED);

    float nccVal = ((float*) res->imageData)[0];
    return nccVal;
}

vector<FBPoint*> Tracker::track(Frame* prev, Frame* curr, int nrOfPoints, vector<CvPoint2D32f*> points) {
    CvSize imageSize = cvSize(prev->width, prev->height);

    IplImage* fromImage = prev->grayscale;
    IplImage* fromPyramid = cvCreateImage( imageSize, 8, 1 );

    IplImage* toImage = curr->grayscale;
    IplImage* toPyramid = cvCreateImage( imageSize, 8, 1 );

    CvPoint2D32f* fromPoints = (CvPoint2D32f*)cvAlloc(nrOfPoints * sizeof(CvPoint2D32f));
    CvPoint2D32f* toPoints = (CvPoint2D32f*)cvAlloc(nrOfPoints * sizeof(CvPoint2D32f));
    CvPoint2D32f* bwPoints = (CvPoint2D32f*)cvAlloc(nrOfPoints * sizeof(CvPoint2D32f));

    char* statusForw = (char*)  cvAlloc(nrOfPoints);
    char* statusBack = (char*)  cvAlloc(nrOfPoints);

    for (int i = 0; i < nrOfPoints; i++) {
        CvPoint2D32f* point = points[i];

        CvPoint2D32f* fPoint = fromPoints + i;
        CvPoint2D32f* tPoint = toPoints + i;
        CvPoint2D32f* bwPoint = bwPoints + i;

        fPoint->x = point->x;
        fPoint->y = point->y;

        tPoint->x = point->x;
        tPoint->y = point->y;

        bwPoint->x = point->x;
        bwPoint->y = point->y;
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
        cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20,0.03),
        CV_LKFLOW_INITIAL_GUESSES | CV_LKFLOW_PYR_A_READY | CV_LKFLOW_PYR_B_READY
    );

    vector<FBPoint*> fbPoints;
    for (int i = 0; i < nrOfPoints; i++) {
        char state = statusForw[i];
        if (state == 1) {
            CvPoint2D32f* fPoint = fromPoints + i;
            CvPoint2D32f* tPoint = toPoints + i;
            CvPoint2D32f* bPoint = bwPoints + i;

            println("P(%a, %a) -> P(%a, %a) -> P(%a, %a)",
                fPoint->x, fPoint->y,
                tPoint->x, tPoint->y,
                bPoint->x, bPoint->y
            );

            float fbError = distance(*fPoint, *bPoint);
            float nccSim = ncc(fromImage, toImage, *fPoint, *tPoint);


            FBPoint* fbPoint = new FBPoint(fPoint, tPoint, bPoint, fbError, nccSim);
            fbPoints.push_back(fbPoint);
        } else {
            fbPoints.push_back(FBPoint::failed);
        }
    }

    return fbPoints;
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
        }
    }
    return dist;
}

bool isOut(Frame* frame, Box* b) {
    return b->x1 > frame->width || b->y1 > frame->height || b->x2 < 0 || b->y2 < 0;
}

Option<Box*> Tracker::estimate(Frame* prev, Frame* curr, Box* box, vector<FBPoint*> trackedPoints, int start, int end) {
    vector<float> fbErrors;
    vector<float> nccErrors;

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
        Option<Box*> failedBox = Option<Box*>();
        return failedBox;
    }

    float medFBE = median(fbErrors);
    float medNCC = median(nccErrors);

    if (medFBE > fbErrorThreshold) {
        printf("Invalid Box -- high fb error - %f\n", medFBE);
        Option<Box*> failedBox = Option<Box*>();
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
        Option<Box*> failedBox = Option<Box*>();
        return failedBox;
    }

    vector<CvPoint2D32f*> fromPoints;
    vector<CvPoint2D32f*> toPoints;

    vector<float> dxList(nrOfReliablePoints);
    vector<float> dyList(nrOfReliablePoints);
    for (int i = 0; i < nrOfReliablePoints; i++) {
        FBPoint* fbPoint = reliablePoints[i];

        CvPoint2D32f* srcPoint = fbPoint->src;
        CvPoint2D32f* targetPoint = fbPoint->to;

        fromPoints.push_back(srcPoint);
        toPoints.push_back(targetPoint);

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
        // println(RED("REAL-TRACKER(OUT) >> %s"), movedBox->toCharArr());
        Option<Box*> failedBox = Option<Box*>();
        return failedBox;
    } else {
        // println(GREEN("REAL-TRACKER(OUT) >> %s"), movedBox->toCharArr());
        Option<Box*> successBox = Option<Box*>(movedBox);
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
            Option<Box*> maybeBox = estimate(prev, curr, stableBox, trackedPoints, cursor, cursor + nrOfPoints);
            if (maybeBox.isDefined()) {
                estimatedBoxList.push_back(maybeBox.get());
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
