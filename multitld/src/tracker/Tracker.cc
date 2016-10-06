#include "tracker/Tracker.hpp"
#include "common/Colors.hpp"
Tracker::Tracker() {
    this->MARGIN = 5;
    this->TERM_CRITERIA = new TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
    this->WIN_SIZE = new Size(4, 4);

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

vector<Box*> Tracker::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    int nrOfBoxes = boxList.size();

    vector<Box*> stableBoxList = getStableBoxes(nrOfBoxes, boxList);
    int nrOfStableBoxes = (int) stableBoxList.size();

    for (int i = 0; i < nrOfStableBoxes; i++) {
        printf("%s\n", stableBoxList[i]->toCharArr());
    }

    if (nrOfStableBoxes == 0) {
        return boxList;
    } else {
        vector<tld::Point*> points = decomposePoints(stableBoxList, nrOfStableBoxes);
        vector<FBPoint*> trackedPoints = track(prev, curr, points);
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
                Point2f point2f = Point2f(box->x1 + i - 2, box->y1 + j);
                tld::Point* point = new tld::Point(point2f);

                nrOfPoints += 1;
                points.push_back(point);
                id++;
            }
        }
        box->nrOfPoints = nrOfPoints;
    }
    return points;
}

vector<FBPoint*> Tracker::track(Frame* prev, Frame* curr, vector<tld::Point*> points) {
    vector<FBPoint*> fbPoints;
    int nrOfPoints = points.size();

    vector<tld::Point*> toPoints = lkTrack(prev, curr, points);
    printf("------\n");
    vector<tld::Point*> backwardPoints = lkTrack(curr, prev, toPoints);

    for (int i = 0; i < nrOfPoints; i++) {
        tld::Point* srcPoint = points[i];
        tld::Point* targetPoint = toPoints[i];
        tld::Point* bwPoint = backwardPoints[i];

        if (bwPoint->state) {
            FBPoint* fbPoint = new FBPoint(srcPoint, targetPoint, bwPoint);
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
            fromPoints[nrOfPoints].x = point->underlying.x;
            fromPoints[nrOfPoints].y = point->underlying.y;

            toPoints[nrOfPoints].x = point->underlying.x;
            toPoints[nrOfPoints].y = point->underlying.y;

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
            CvPoint2D32f toPoint2f = toPoints[id];
            Point2f toPoint = Point2f(toPoint2f.x, toPoint2f.y);
            uchar state = status[id];
            if (state) {
                targetPoint = new tld::Point(toPoint);
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


float computeFBError(tld::Point* src, tld::Point* target, tld::Point* fb) {
    float dy = fb->underlying.y - src->underlying.y;
    float dx = fb->underlying.x - src->underlying.x;
    float fbe = sqrt(dx*dx + dy*dy);
    return fbe;
}

float computeNCC(Frame* srcFrame, tld::Point* srcPoint, Frame* targetFrame, tld::Point* targetPoint) {
    //TODO Might need to write own ncc computation
    Size windowSize(4, 4);
    Mat rec0 = Mat::zeros(windowSize, CV_8U);
    Mat rec1 = Mat::zeros(windowSize, CV_8U);
    Mat res = Mat::zeros(Size(1, 1), CV_32F);

    getRectSubPix(*(targetFrame->grayscale), windowSize, targetPoint->underlying, rec0);
    getRectSubPix(*(srcFrame->grayscale), windowSize, srcPoint->underlying, rec1);
    matchTemplate(rec0, rec1, res, CV_TM_CCOEFF_NORMED );
    float ncc = ((float *)(res.data))[0];
    return ncc;
}

float median(vector<float> floatVector) {
    int size = (int) floatVector.size();
    sort(floatVector.begin(), floatVector.end(), [](float a, float b) { return a > b; });
    return floatVector[size / 2];
}

Option<Box>* estimate(Frame* prev, Frame* curr, Box* box, vector<FBPoint*> trackedPoints, int start, int end) {
    vector<float> fbErrors;
    vector<float> nccErrors;
    int nrOfStablePoints = 0;
    for (int i = start; i < end; i++) {
        FBPoint* fbPoint = trackedPoints[i];
        if (fbPoint->state) {
            nrOfStablePoints += 1;
            tld::Point* srcPoint = fbPoint->src;
            tld::Point* targetPoint = fbPoint->to;
            tld::Point* bwPoint = fbPoint->backwardPoint;

            float fbError = computeFBError(srcPoint, targetPoint, bwPoint);
            float ncc = computeNCC(prev, srcPoint, curr, targetPoint);

            fbPoint->fbError = fbError;
            fbPoint->ncc = ncc;

            fbErrors.push_back(fbError);
            nccErrors.push_back(ncc);
        }
    }

    if (nrOfStablePoints == 0) {
        printf("Invalid Box -- No Stable Point - %d\n", nrOfStablePoints);
        Option<Box>* failedBox = new Option<Box>();
        return failedBox;
    }

    float medFBE = median(fbErrors);
    float medNCC = median(nccErrors);

    if (medFBE > 10) {
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

    vector<float> dxList(nrOfReliablePoints);
    vector<float> dyList(nrOfReliablePoints);
    for (int i = 0; i < nrOfReliablePoints; i++) {
        FBPoint* fbPoint = reliablePoints[i];
        dxList[i] = fbPoint->dx;
        dyList[i] = fbPoint->dy;
    }

    float medX = median(dxList);
    float medY = median(dyList);

    Box* movedBox = box->move(medX, medY);
    Option<Box>* successBox = new Option<Box>(movedBox);
    return successBox;
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
