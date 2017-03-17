
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "opencv2/video/tracking.hpp"

#include "common/Logging.hpp"
#include "core/Point.hpp"

void track(IplImage* fromImage, IplImage* toImage, int nrOfPoints, vector<tld::Point*> points);

int main(int argc, char** argv) {
    IplImage* fromImage = cvLoadImage(
        "/Users/mehmetgunturkun/MasterThesis/ceng500-mscthesis/resources/04_pedestrian2/sequence/00002.jpg",
        CV_LOAD_IMAGE_GRAYSCALE
    );

    IplImage* toImage = cvLoadImage(
        "/Users/mehmetgunturkun/MasterThesis/ceng500-mscthesis/resources/04_pedestrian2/sequence/00003.jpg",
        CV_LOAD_IMAGE_GRAYSCALE
    );

    vector<tld::Point*> pointList = {
        new tld::Point(0x1.0d09cep+7, 0x1.142d2ap+7),
        new tld::Point(0x1.0f8f8cp+7, 0x1.142d2ap+7),
        new tld::Point(0x1.12154ap+7, 0x1.142d2ap+7),
        new tld::Point(0x1.149b08p+7, 0x1.142d2ap+7),
        new tld::Point(0x1.1720c6p+7, 0x1.142d2ap+7),
        new tld::Point(0x1.19a684p+7, 0x1.142d2ap+7),
        new tld::Point(0x1.1c2c42p+7, 0x1.142d2ap+7),
        new tld::Point(0x1.1eb2p+7, 0x1.142d2ap+7),
        new tld::Point(0x1.2137bcp+7, 0x1.142d2ap+7),
        new tld::Point(0x1.23bd7ap+7, 0x1.142d2ap+7),
        new tld::Point(0x1.0d09cep+7, 0x1.1dec64p+7),
        new tld::Point(0x1.0f8f8cp+7, 0x1.1dec64p+7),
        new tld::Point(0x1.12154ap+7, 0x1.1dec64p+7),
        new tld::Point(0x1.149b08p+7, 0x1.1dec64p+7),
        new tld::Point(0x1.1720c6p+7, 0x1.1dec64p+7),
        new tld::Point(0x1.19a684p+7, 0x1.1dec64p+7),
        new tld::Point(0x1.1c2c42p+7, 0x1.1dec64p+7),
        new tld::Point(0x1.1eb2p+7, 0x1.1dec64p+7),
        new tld::Point(0x1.2137bcp+7, 0x1.1dec64p+7),
        new tld::Point(0x1.23bd7ap+7, 0x1.1dec64p+7),
        new tld::Point(0x1.0d09cep+7, 0x1.27ab9cp+7),
        new tld::Point(0x1.0f8f8cp+7, 0x1.27ab9cp+7),
        new tld::Point(0x1.12154ap+7, 0x1.27ab9cp+7),
        new tld::Point(0x1.149b08p+7, 0x1.27ab9cp+7),
        new tld::Point(0x1.1720c6p+7, 0x1.27ab9cp+7),
        new tld::Point(0x1.19a684p+7, 0x1.27ab9cp+7),
        new tld::Point(0x1.1c2c42p+7, 0x1.27ab9cp+7),
        new tld::Point(0x1.1eb2p+7, 0x1.27ab9cp+7),
        new tld::Point(0x1.2137bcp+7, 0x1.27ab9cp+7),
        new tld::Point(0x1.23bd7ap+7, 0x1.27ab9cp+7),
        new tld::Point(0x1.0d09cep+7, 0x1.316ad4p+7),
        new tld::Point(0x1.0f8f8cp+7, 0x1.316ad4p+7),
        new tld::Point(0x1.12154ap+7, 0x1.316ad4p+7),
        new tld::Point(0x1.149b08p+7, 0x1.316ad4p+7),
        new tld::Point(0x1.1720c6p+7, 0x1.316ad4p+7),
        new tld::Point(0x1.19a684p+7, 0x1.316ad4p+7),
        new tld::Point(0x1.1c2c42p+7, 0x1.316ad4p+7),
        new tld::Point(0x1.1eb2p+7, 0x1.316ad4p+7),
        new tld::Point(0x1.2137bcp+7, 0x1.316ad4p+7),
        new tld::Point(0x1.23bd7ap+7, 0x1.316ad4p+7),
        new tld::Point(0x1.0d09cep+7, 0x1.3b2a0ep+7),
        new tld::Point(0x1.0f8f8cp+7, 0x1.3b2a0ep+7),
        new tld::Point(0x1.12154ap+7, 0x1.3b2a0ep+7),
        new tld::Point(0x1.149b08p+7, 0x1.3b2a0ep+7),
        new tld::Point(0x1.1720c6p+7, 0x1.3b2a0ep+7),
        new tld::Point(0x1.19a684p+7, 0x1.3b2a0ep+7),
        new tld::Point(0x1.1c2c42p+7, 0x1.3b2a0ep+7),
        new tld::Point(0x1.1eb2p+7, 0x1.3b2a0ep+7),
        new tld::Point(0x1.2137bcp+7, 0x1.3b2a0ep+7),
        new tld::Point(0x1.23bd7ap+7, 0x1.3b2a0ep+7),
        new tld::Point(0x1.0d09cep+7, 0x1.44e946p+7),
        new tld::Point(0x1.0f8f8cp+7, 0x1.44e946p+7),
        new tld::Point(0x1.12154ap+7, 0x1.44e946p+7),
        new tld::Point(0x1.149b08p+7, 0x1.44e946p+7),
        new tld::Point(0x1.1720c6p+7, 0x1.44e946p+7),
        new tld::Point(0x1.19a684p+7, 0x1.44e946p+7),
        new tld::Point(0x1.1c2c42p+7, 0x1.44e946p+7),
        new tld::Point(0x1.1eb2p+7, 0x1.44e946p+7),
        new tld::Point(0x1.2137bcp+7, 0x1.44e946p+7),
        new tld::Point(0x1.23bd7ap+7, 0x1.44e946p+7),
        new tld::Point(0x1.0d09cep+7, 0x1.4ea88p+7),
        new tld::Point(0x1.0f8f8cp+7, 0x1.4ea88p+7),
        new tld::Point(0x1.12154ap+7, 0x1.4ea88p+7),
        new tld::Point(0x1.149b08p+7, 0x1.4ea88p+7),
        new tld::Point(0x1.1720c6p+7, 0x1.4ea88p+7),
        new tld::Point(0x1.19a684p+7, 0x1.4ea88p+7),
        new tld::Point(0x1.1c2c42p+7, 0x1.4ea88p+7),
        new tld::Point(0x1.1eb2p+7, 0x1.4ea88p+7),
        new tld::Point(0x1.2137bcp+7, 0x1.4ea88p+7),
        new tld::Point(0x1.23bd7ap+7, 0x1.4ea88p+7),
        new tld::Point(0x1.0d09cep+7, 0x1.5867b8p+7),
        new tld::Point(0x1.0f8f8cp+7, 0x1.5867b8p+7),
        new tld::Point(0x1.12154ap+7, 0x1.5867b8p+7),
        new tld::Point(0x1.149b08p+7, 0x1.5867b8p+7),
        new tld::Point(0x1.1720c6p+7, 0x1.5867b8p+7),
        new tld::Point(0x1.19a684p+7, 0x1.5867b8p+7),
        new tld::Point(0x1.1c2c42p+7, 0x1.5867b8p+7),
        new tld::Point(0x1.1eb2p+7, 0x1.5867b8p+7),
        new tld::Point(0x1.2137bcp+7, 0x1.5867b8p+7),
        new tld::Point(0x1.23bd7ap+7, 0x1.5867b8p+7),
        new tld::Point(0x1.0d09cep+7, 0x1.6226f2p+7),
        new tld::Point(0x1.0f8f8cp+7, 0x1.6226f2p+7),
        new tld::Point(0x1.12154ap+7, 0x1.6226f2p+7),
        new tld::Point(0x1.149b08p+7, 0x1.6226f2p+7),
        new tld::Point(0x1.1720c6p+7, 0x1.6226f2p+7),
        new tld::Point(0x1.19a684p+7, 0x1.6226f2p+7),
        new tld::Point(0x1.1c2c42p+7, 0x1.6226f2p+7),
        new tld::Point(0x1.1eb2p+7, 0x1.6226f2p+7),
        new tld::Point(0x1.2137bcp+7, 0x1.6226f2p+7),
        new tld::Point(0x1.23bd7ap+7, 0x1.6226f2p+7),
        new tld::Point(0x1.0d09cep+7, 0x1.6be62ap+7),
        new tld::Point(0x1.0f8f8cp+7, 0x1.6be62ap+7),
        new tld::Point(0x1.12154ap+7, 0x1.6be62ap+7),
        new tld::Point(0x1.149b08p+7, 0x1.6be62ap+7),
        new tld::Point(0x1.1720c6p+7, 0x1.6be62ap+7),
        new tld::Point(0x1.19a684p+7, 0x1.6be62ap+7),
        new tld::Point(0x1.1c2c42p+7, 0x1.6be62ap+7),
        new tld::Point(0x1.1eb2p+7, 0x1.6be62ap+7),
        new tld::Point(0x1.2137bcp+7, 0x1.6be62ap+7),
        new tld::Point(0x1.23bd7ap+7, 0x1.6be62ap+7)
    };

    int nrOfPoints = (int) pointList.size();

    // cvShowImage("from", fromImage);
    // cvShowImage("to", toImage);
    // cvWaitKey(0);

    track(fromImage, toImage, nrOfPoints, pointList);
}

// float distance(CvPoint2D32f p0, CvPoint2D32f p1) {
//     float distanceVal = sqrt((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y));
//     return distanceVal;
// }
//
// float ncc(IplImage* im0, IplImage* im1, CvPoint2D32f p0, CvPoint2D32f p1) {
//     IplImage* rec0 = cvCreateImage(cvSize(10, 10), 8, 1);
//     IplImage* rec1 = cvCreateImage(cvSize(10, 10), 8, 1);
//     IplImage* res = cvCreateImage(cvSize(1, 1), IPL_DEPTH_32F, 1);
//
//     cvGetRectSubPix(im0, rec0, p0);
//     cvGetRectSubPix(im1, rec1, p1);
//
//     cvMatchTemplate(rec0, rec1, res, CV_TM_CCOEFF_NORMED);
//     float nccVal = ((float*) res->imageData)[0];
//     return nccVal;
// }

void track(IplImage* fromImage, IplImage* toImage, int nrOfPoints, vector<tld::Point*> points) {
    println("H: %d, W: %d, NrOfCh: %d", fromImage->height, fromImage->width)
    CvSize imageSize = cvSize(fromImage->width, fromImage->height);

    // IplImage* fromImage = new IplImage(*(prev->grayscale));
    IplImage* fromPyramid = cvCreateImage( imageSize, 8, 1 );

    // IplImage* toImage = new IplImage(*(curr->grayscale));
    IplImage* toPyramid = cvCreateImage( imageSize, 8, 1 );

    CvPoint2D32f* fromPoints = (CvPoint2D32f*) cvAlloc(nrOfPoints * sizeof(CvPoint2D32f));
    CvPoint2D32f* toPoints = (CvPoint2D32f*) cvAlloc(nrOfPoints * sizeof(CvPoint2D32f));
    CvPoint2D32f* bwPoints = (CvPoint2D32f*) cvAlloc(nrOfPoints * sizeof(CvPoint2D32f));

    char* statusForw = (char*) cvAlloc(nrOfPoints);
    char* statusBack = (char*) cvAlloc(nrOfPoints);

    for (int i = 0; i < nrOfPoints; i++) {
        tld::Point* point = points[i];

        CvPoint2D32f* fPoint = fromPoints + i;
        CvPoint2D32f* tPoint = toPoints + i;
        CvPoint2D32f* bwPoint = bwPoints + i;

        fPoint->x = point->underlying->x;
        fPoint->y = point->underlying->y;

        tPoint->x = point->underlying->x;
        tPoint->y = point->underlying->y;
        println("new tld::Point(%a, %a)", fPoint->x, fPoint->y);

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

    println("Tracking is completed");

    // vector<FBPoint*> fbPoints;
    for (int i = 0; i < nrOfPoints; i++) {
        int state = (int) statusForw[i];
        if (state == 1) {
            CvPoint2D32f* fPoint = fromPoints + i;
            tld::Point* fromPoint = new tld::Point(fPoint);

            CvPoint2D32f* tPoint = toPoints + i;
            tld::Point* toPoint = new tld::Point(tPoint);

            CvPoint2D32f* bPoint = bwPoints + i;
            tld::Point* bwPoint = new tld::Point(bPoint);

            // float fbError = distance(*fPoint, *bPoint);

            // float nccSim = ncc(fromImage, toImage, *fPoint, *tPoint);

            println("P(%a, %a) -> P(%a, %a) -> P(%a, %a)",
                fPoint->x, fPoint->y,
                tPoint->x, tPoint->y,
                bPoint->x, bPoint->y
            );


            // FBPoint* fbPoint = new FBPoint(fromPoint, toPoint, bwPoint, fbError, nccSim);
            // fbPoints.push_back(fbPoint);
        } else {
            // fbPoints.push_back(FBPoint::failed);
        }
    }
}
