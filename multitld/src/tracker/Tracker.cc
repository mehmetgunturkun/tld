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
        printf("PRE >> %s\n", stableBoxList[i]->toCharArr());
    }

    vector<Box*> nextBoxList;
    if (nrOfStableBoxes == 0) {
        nextBoxList = boxList;
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
        nextBoxList = estimatedBoxList;
    }

    for (int i = 0; i < nextBoxList.size(); i++) {
        if (nextBoxList[i] != nullptr) {
            printf("POST >> %s\n", nextBoxList[i]->toCharArr());
        }
    }
    return nextBoxList;
}


double Tracker::computeStep(double start, double end, int pointCount) {
    double step = ((end - 5.0) - (start + 5.0)) / (pointCount - 1);
    return step;
}

vector<tld::Point*> Tracker::decomposePoints(vector<Box*> boxList, int nrOfBoxes) {
    vector<tld::Point*> points;
    int id = 0;
    for (int i = 0; i < nrOfBoxes; i++) {
        Box* box = boxList[i];
        int nrOfPoints = 0;

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

        for (double i = 5; i <= widthWithPadding; i = i + horizontalStep) {
            for (double j = 5; j <= heightWithPadding; j = j + verticalStep) {
                double x = x1 + i;
                double y = y1 + j;

                float xf = (float) x;
                float yf = (float) y;

                // printf("points.push_back(Point2f(%f, %f));\n", xf, yf);

                Point2f point2f = Point2f(xf, yf);
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
    printf("-------------------------------------------------------\n");
    vector<tld::Point*> backwardPoints = lkTrack(curr, prev, toPoints);

    for (int i = 0; i < nrOfPoints; i++) {
        tld::Point* srcPoint = points[i];
        tld::Point* targetPoint = toPoints[i];
        tld::Point* bwPoint = backwardPoints[i];

        if (targetPoint->state) {
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
        fromPoints[nrOfPoints].x = point->underlying.x;
        fromPoints[nrOfPoints].y = point->underlying.y;

        toPoints[nrOfPoints].x = point->underlying.x;
        toPoints[nrOfPoints].y = point->underlying.y;

        nrOfPoints += 1;
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
        CV_LKFLOW_INITIAL_GUESSES);

    vector<tld::Point*> targetPoints;
    int id = 0;
    for (int i = 0; i < pointCount; i++) {
        tld::Point* srcPoint = srcPoints[i];
        tld::Point* targetPoint;
        // if (srcPoint->state) {
            CvPoint2D32f toPoint2f = toPoints[id];

            Point2f toPoint = Point2f(toPoint2f.x, toPoint2f.y);
            int state = (int) status[id];
            if (state == 1) {
                printf(GREEN("PO(%f, %f) >> P1(%f, %f)\n"), srcPoint->underlying.x, srcPoint->underlying.y, toPoint2f.x, toPoint2f.y);
                targetPoint = new tld::Point(toPoint);
            } else {
                printf(RED("PO(%f, %f) >> P1(%f, %f)\n"), srcPoint->underlying.x, srcPoint->underlying.y, toPoint2f.x, toPoint2f.y);
                targetPoint = tld::Point::failed;
            }
            id += 1;
        // } else {
        //     targetPoint = tld::Point::failed;
        // }
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
    if (size == 0) {
        printf(RED("There is no item to get median\n"));
        return 0.0f;
    } else if (size == 1) {
        return floatVector[0];
    } else {
        sort(floatVector.begin(), floatVector.end(), [](float a, float b) { return a > b; });
        int middle = size / 2;
        float med = floatVector[middle];

        printf(CYAN("MED(%d): %f\n"), middle, med);

        if (size % 2 == 0) {
            printf(CYAN("MED(%d): %f\n"), middle - 1, floatVector[middle - 1]);

            med = (med + floatVector[middle - 1]) / 2;
        }

        return med;
    }
}

double mediand(vector<double> floatVector) {
    int size = (int) floatVector.size();
    if (size == 0) {
        printf(RED("There is no item to get median\n"));
        return 0.0f;
    } else if (size == 1) {
        return floatVector[0];
    } else {
        sort(floatVector.begin(), floatVector.end(), [](float a, float b) { return a > b; });
        int middle = size / 2;
        double med = floatVector[middle];

        printf(CYAN("MED(%d): %f\n"), middle, med);

        if (size % 2 == 0) {

            printf(CYAN("MED(%d): %f\n"), middle - 1, floatVector[middle - 1]);
            med = (med + floatVector[middle - 1]) / 2;
        }

        return med;
    }
}

Option<Box>* estimate(Frame* prev, Frame* curr, Box* box, vector<FBPoint*> trackedPoints, int start, int end) {
    vector<double> fbErrors;
    vector<float> nccErrors;
    int nrOfStablePoints = 0;
    for (int i = start; i < end; i++) {
        FBPoint* fbPoint = trackedPoints[i];
        if (fbPoint->state) {
            nrOfStablePoints += 1;
            tld::Point* srcPoint = fbPoint->src;
            tld::Point* targetPoint = fbPoint->to;
            tld::Point* bwPoint = fbPoint->backwardPoint;

            double x1 = (double) srcPoint->underlying.x;
            double y1 = (double) srcPoint->underlying.y;
            double x3 = (double) bwPoint->underlying.x;
            double y3 = (double) bwPoint->underlying.y;

            double dx = x3 - x1;
            double dy = y3 - y1;
            double error = sqrt(dx*dx + dy*dy);

            float fbError = computeFBError(srcPoint, targetPoint, bwPoint);
            float ncc = computeNCC(prev, srcPoint, curr, targetPoint);

            printf(CYAN("P(x = %f, y = %f, fbb = %f, ncc = %f)\n"),
                targetPoint->underlying.x,
                targetPoint->underlying.y,
                fbError,
                ncc);



            fbPoint->fbError = error;
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

    double medFBE = mediand(fbErrors);
    float medNCC = median(nccErrors);

    printf("median(fb): %g\n", medFBE);
    printf("median(ncc): %f\n", medNCC);

    if (medFBE > 10) {
        printf("Invalid Box -- high fb error - %f\n", medFBE);
        Option<Box>* failedBox = new Option<Box>();
        return failedBox;
    }

    vector<FBPoint*> reliablePoints;
    for (int i = start; i < end; i++) {
        FBPoint* fbPoint = trackedPoints[i];
        if (fbPoint->state && fbPoint->fbError <= 0.0284 && fbPoint->ncc >= 0.98) {
            reliablePoints.push_back(fbPoint);
        }
    }

    int nrOfReliablePoints = reliablePoints.size();

    printf("Nr Of Reliable Points: %d\n", nrOfReliablePoints);
    if (nrOfReliablePoints == 0) {
        printf("No reliable boxes\n");
        Option<Box>* failedBox = new Option<Box>();
        return failedBox;
    }

    vector<double> dxList(nrOfReliablePoints);
    vector<double> dyList(nrOfReliablePoints);
    for (int i = 0; i < nrOfReliablePoints; i++) {
        FBPoint* fbPoint = reliablePoints[i];
        dxList[i] = (double) fbPoint->dx;
        dyList[i] = (double) fbPoint->dy;
    }

    double medX = mediand(dxList);
    double medY = mediand(dyList);

    printf("dx: %f, dy: %f\n", medX, medY);

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
