#include "common/Arguments.hpp"

#include "tracker/Tracker.hpp"
#include "testbase/Sequence.hpp"

float ncc(IplImage* im0, IplImage* im1) {
    CvPoint2D32f p0 = CvPoint2D32f();
    // p0.x = 0x1p+7;
    // p0.y = 0x1.18p+7;

    p0.x = 100.0f;
    p0.y = 100.0f;

    CvPoint2D32f p1 = CvPoint2D32f();
    // p1.x = 0x1.0d63a4p+7;
    // p1.y = 0x1.14b08p+7;

    p1.x = 100.0f;
    p1.y = 100.0f;

    IplImage* rec0 = cvCreateImage(cvSize(10, 10), 8, 1);
    IplImage* rec1 = cvCreateImage(cvSize(10, 10), 8, 1);
    IplImage* res = cvCreateImage(cvSize(1, 1), 32, 1);


    cvGetRectSubPix(im0, rec0, p0);
    cvGetRectSubPix(im1, rec1, p1);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            char pixel = rec0->imageData[i * 10 + j];
            printf("%4d ", (int) pixel);
        }
        println("");
    }

    println("----------------------");

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            char pixel = rec1->imageData[i * 10 + j];
            printf("%4d ", (int) pixel);
        }
        println("");
    }

    cvMatchTemplate(rec0, rec1, res, CV_TM_CCOEFF_NORMED);

    float nccVal = ((float*) res->imageData)[0];
    return nccVal;
}


int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);
    string sequenceKey = args->getString("sequence");

    Sequence* sequence = new Sequence(sequenceKey);


    Mat im1 = cv::imread("/Users/mehmetgunturkun/MasterThesis/ceng500-mscthesis/resources/04_pedestrian2/sequence/00001.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    Mat im2 = cv::imread("/Users/mehmetgunturkun/MasterThesis/ceng500-mscthesis/resources/04_pedestrian2/sequence/00002.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    IplImage* iplImage1 = new IplImage(im1);
    IplImage* iplImage2 = new IplImage(im2);

    println("mc1000");
    ncc(iplImage1, iplImage2);
    println("mc2000");

    Tracker* tracker = new Tracker();

    Frame* frame1 = sequence->get(0);
    Frame* frame2 = sequence->get(1);

    vector<tld::Point*> pointList = {
        new tld::Point(127.0f,141.0f),
        new tld::Point(128.22222900390625f,141.0f),
        new tld::Point(129.4444427490234375f,141.0f),
        new tld::Point(130.6666717529296875f,141.0f),
        new tld::Point(131.888885498046875f,141.0f),
        new tld::Point(133.111114501953125f,141.0f),
        new tld::Point(134.3333282470703125f,141.0f),
        new tld::Point(135.5555572509765625f,141.0f),
        new tld::Point(136.77777099609375f,141.0f),
        new tld::Point(138.0f,141.0f),
        new tld::Point(127.0f,145.77777099609375f),
        new tld::Point(128.22222900390625f,145.77777099609375f),
        new tld::Point(129.4444427490234375f,145.77777099609375f),
        new tld::Point(130.6666717529296875f,145.77777099609375f),
        new tld::Point(131.888885498046875f,145.77777099609375f),
        new tld::Point(133.111114501953125f,145.77777099609375f),
        new tld::Point(134.3333282470703125f,145.77777099609375f),
        new tld::Point(135.5555572509765625f,145.77777099609375f),
        new tld::Point(136.77777099609375f,145.77777099609375f),
        new tld::Point(138.0f,145.77777099609375f),
        new tld::Point(127.0f,150.5555572509765625f),
        new tld::Point(128.22222900390625f,150.5555572509765625f),
        new tld::Point(129.4444427490234375f,150.5555572509765625f),
        new tld::Point(130.6666717529296875f,150.5555572509765625f),
        new tld::Point(131.888885498046875f,150.5555572509765625f),
        new tld::Point(133.111114501953125f,150.5555572509765625f),
        new tld::Point(134.3333282470703125f,150.5555572509765625f),
        new tld::Point(135.5555572509765625f,150.5555572509765625f),
        new tld::Point(136.77777099609375f,150.5555572509765625f),
        new tld::Point(138.0f,150.5555572509765625f),
        new tld::Point(127.0f,155.3333282470703125f),
        new tld::Point(128.22222900390625f,155.3333282470703125f),
        new tld::Point(129.4444427490234375f,155.3333282470703125f),
        new tld::Point(130.6666717529296875f,155.3333282470703125f),
        new tld::Point(131.888885498046875f,155.3333282470703125f),
        new tld::Point(133.111114501953125f,155.3333282470703125f),
        new tld::Point(134.3333282470703125f,155.3333282470703125f),
        new tld::Point(135.5555572509765625f,155.3333282470703125f),
        new tld::Point(136.77777099609375f,155.3333282470703125f),
        new tld::Point(138.0f,155.3333282470703125f),
        new tld::Point(127.0f,160.111114501953125f),
        new tld::Point(128.22222900390625f,160.111114501953125f),
        new tld::Point(129.4444427490234375f,160.111114501953125f),
        new tld::Point(130.6666717529296875f,160.111114501953125f),
        new tld::Point(131.888885498046875f,160.111114501953125f),
        new tld::Point(133.111114501953125f,160.111114501953125f),
        new tld::Point(134.3333282470703125f,160.111114501953125f),
        new tld::Point(135.5555572509765625f,160.111114501953125f),
        new tld::Point(136.77777099609375f,160.111114501953125f),
        new tld::Point(138.0f,160.111114501953125f),
        new tld::Point(127.0f,164.888885498046875f),
        new tld::Point(128.22222900390625f,164.888885498046875f),
        new tld::Point(129.4444427490234375f,164.888885498046875f),
        new tld::Point(130.6666717529296875f,164.888885498046875f),
        new tld::Point(131.888885498046875f,164.888885498046875f),
        new tld::Point(133.111114501953125f,164.888885498046875f),
        new tld::Point(134.3333282470703125f,164.888885498046875f),
        new tld::Point(135.5555572509765625f,164.888885498046875f),
        new tld::Point(136.77777099609375f,164.888885498046875f),
        new tld::Point(138.0f,164.888885498046875f),
        new tld::Point(127.0f,169.6666717529296875f),
        new tld::Point(128.22222900390625f,169.6666717529296875f),
        new tld::Point(129.4444427490234375f,169.6666717529296875f),
        new tld::Point(130.6666717529296875f,169.6666717529296875f),
        new tld::Point(131.888885498046875f,169.6666717529296875f),
        new tld::Point(133.111114501953125f,169.6666717529296875f),
        new tld::Point(134.3333282470703125f,169.6666717529296875f),
        new tld::Point(135.5555572509765625f,169.6666717529296875f),
        new tld::Point(136.77777099609375f,169.6666717529296875f),
        new tld::Point(138.0f,169.6666717529296875f),
        new tld::Point(127.0f,174.4444427490234375f),
        new tld::Point(128.22222900390625f,174.4444427490234375f),
        new tld::Point(129.4444427490234375f,174.4444427490234375f),
        new tld::Point(130.6666717529296875f,174.4444427490234375f),
        new tld::Point(131.888885498046875f,174.4444427490234375f),
        new tld::Point(133.111114501953125f,174.4444427490234375f),
        new tld::Point(134.3333282470703125f,174.4444427490234375f),
        new tld::Point(135.5555572509765625f,174.4444427490234375f),
        new tld::Point(136.77777099609375f,174.4444427490234375f),
        new tld::Point(138.0f,174.4444427490234375f),
        new tld::Point(127.0f,179.22222900390625f),
        new tld::Point(128.22222900390625f,179.22222900390625f),
        new tld::Point(129.4444427490234375f,179.22222900390625f),
        new tld::Point(130.6666717529296875f,179.22222900390625f),
        new tld::Point(131.888885498046875f,179.22222900390625f),
        new tld::Point(133.111114501953125f,179.22222900390625f),
        new tld::Point(134.3333282470703125f,179.22222900390625f),
        new tld::Point(135.5555572509765625f,179.22222900390625f),
        new tld::Point(136.77777099609375f,179.22222900390625f),
        new tld::Point(138.0f,179.22222900390625f),
        new tld::Point(127.0f,184.0f),
        new tld::Point(128.22222900390625f,184.0f),
        new tld::Point(129.4444427490234375f,184.0f),
        new tld::Point(130.6666717529296875f,184.0f),
        new tld::Point(131.888885498046875f,184.0f),
        new tld::Point(133.111114501953125f,184.0f),
        new tld::Point(134.3333282470703125f,184.0f),
        new tld::Point(135.5555572509765625f,184.0f),
        new tld::Point(136.77777099609375f,184.0f),
        new tld::Point(138.0f,184.0f)
    };

    int nrOfPoints = (int) pointList.size();
    // tld::Point* firstPoint = pointList[0];
    //
    // std::stringstream ss;
    // ss << std::setprecision( std::numeric_limits<int>::max() );
    // ss << "P(";
    // ss << firstPoint->underlying->x;
    // ss << ",";
    // ss << firstPoint->underlying->y;
    // ss << ")" << endl;
    // string res = ss.str();
    //
    // cout << res;

    // vector<FBPoint*> trackedPoints = tracker->track(frame1, frame2, nrOfPoints, pointList);

    return EXIT_SUCCESS;
}
