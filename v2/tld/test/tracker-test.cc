#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"

#include "tracker/Tracker.hpp"

float ncc(IplImage* im0, IplImage* im1);
int run(Arguments* args);

int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);
    for (int i = 0; i < 1; i++) {
        run(args);
    }
}

int run(Arguments* args) {
    string sequenceKey = args->getString("sequence");
    Sequence sequence = Sequence(sequenceKey);

    Frame* prevFrame = sequence.next();
    Box* initBox = sequence.initBox;
    vector<Box*> boxList = { initBox };

    Mat im1 = cv::imread("/Users/mehmetgunturkun/MasterThesis/ceng500-mscthesis/resources/04_pedestrian2/sequence/00001.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    Mat im2 = cv::imread("/Users/mehmetgunturkun/MasterThesis/ceng500-mscthesis/resources/04_pedestrian2/sequence/00002.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    IplImage* iplImage1 = new IplImage(im1);
    IplImage* iplImage2 = new IplImage(im2);
    try
    {
        ncc(iplImage1, iplImage2);
    }
    catch( cv::Exception& e )
    {
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
    }


    Tracker* tracker = new Tracker();

    while (sequence.hasNext()) {
        Frame* currentFrame = sequence.next();
        println("%s is going to be processed", currentFrame->name.c_str());



        boxList = tracker->track(prevFrame, currentFrame, boxList);

        println("%s", boxList[0]->toCharArr());

        prevFrame = currentFrame;
    }

    return 0;
}

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
    IplImage* res = cvCreateImage(cvSize(1, 1), IPL_DEPTH_32F, 1);


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
