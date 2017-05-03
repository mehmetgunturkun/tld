
#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"
#include "core/Gaussian.hpp"

using namespace cv;


int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);
    Sequence sequence = Sequence(args.getString("sequence"));

    for (int i = 0; i < 1; i++) {
        Frame* frame = sequence.next();
        println("%s", frame->name.c_str());

        IplImage* grayscale = frame->grayscale;
        // IplImage* img = cvLoadImage("/home/gunturkun/MasterThesis/resources/04_pedestrian2/sequence/00001.jpg", CV_LOAD_IMAGE_GRAYSCALE);
        Mat blurred = Gaussian::blur(grayscale, 12, 2.0);
        // Mat m = Mat(*blurred);
        imshow("BLUR", blurred);
        waitKey(0);

        // delete frame;
    }

    return 0;
}
