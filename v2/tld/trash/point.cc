#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "common/Arguments.hpp"
#include "common/Colors.hpp"

#include <iostream>

using namespace std;
using namespace cv;

//g++ point.cc -I/opt/opencv/2.2/include -L/opt/opencv/2.2/lib -lopencv_highgui -lopencv_core -o point

int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);

    string value = args->getString("sequence");

    cout << "OpenCV version : " << CV_VERSION << endl;
    cout << "Major version : " << CV_MAJOR_VERSION << endl;
    cout << "Minor version : " << CV_MINOR_VERSION << endl;
    cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;

    // IplImage* image = cvLoadImage("/tmp/test.pgm");
    // cvShowImage("test", image);
    // cvWaitKey(0);

    Mat image = imread("/tmp/test.pgm");
    imshow("test1", image);
    waitKey(0);

    Mat grayImage;
    cvtColor(image, grayImage, CV_LOAD_IMAGE_GRAYSCALE);


    IplImage* iplImage = new IplImage(image);
    int height = image.rows;
    int width = image.cols;
    int nrOfChannels = image.channels();

    printf("H: %d, W: %d, NrOfCh: %d\n", height, width, nrOfChannels);

    char* imageData = iplImage->imageData;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            imageData[i * width + j] = 0;
        }
    }

    cvShowImage("blackout", iplImage);
    cvWaitKey(0);

    imshow("isBlackedout", image);
    waitKey(0);
}
