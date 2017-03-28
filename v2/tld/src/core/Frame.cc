#include "core/Frame.hpp"

Frame::Frame(int id, string name, IplImage* originalImage, IplImage* grayscale, IplImage* gaussian) {
    this->id = id;
    this->name = name;

    this->originalImage = originalImage;
    this->grayscale = grayscale;
    this->gaussian = gaussian;

    this->integralImage = new IntegralImage(this->grayscale);

    this->width = originalImage->width;
    this->height = originalImage->height;
}

Frame::~Frame() {
    cvReleaseImage(&originalImage);
    cvReleaseImage(&grayscale);

    //TODO Check the tru way to release gaussian image.
    delete gaussian;
    delete integralImage;
}

int Frame::get(Point2f* point) {
    int pixel = (int) CV_IMAGE_ELEM(gaussian, uchar, (int) point->y, (int) point->x);
    return pixel;
}

Frame* Frame::clone() {
    IplImage* cloneOriginalImage = cvCloneImage(originalImage);
    IplImage* cloneGrayscale = cvCloneImage(grayscale);
    IplImage* cloneGaussian = cvCloneImage(gaussian);

    string cloneFileName = this->name + "_copy";
    Frame* cloneFrame = new Frame(
        this->id,
        cloneFileName,
        cloneOriginalImage,
        cloneGrayscale,
        cloneGaussian
    );
    return cloneFrame;
}

Option<Frame*> Frame::fromFile(int id, string fileName) {
    IplImage* originalImage = Image::imread(fileName, CV_LOAD_IMAGE_ANYCOLOR);
    IplImage* grayImage = Image::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);

    long total = 0;
    for (int i = 0; i < grayImage->height; i++) {
        for (int j = 0; j < grayImage->width; j++) {
            uchar pixel = CV_IMAGE_ELEM(grayImage, uchar, i, j);
            total += pixel;
        }
    }
    printf("Gray : %10lu\n", total);

    IplImage* gray4Blur = Image::bgr2gray(originalImage);
    IplImage* gaussianImage = Gaussian::blur(gray4Blur, 12, 2.0);
    total = 0;
    for (int i = 0; i < grayImage->height; i++) {
        for (int j = 0; j < grayImage->width; j++) {
            uchar pixel = CV_IMAGE_ELEM(gaussianImage, uchar, i, j);
            total += pixel;
        }
    }
    printf("Gauss: %10lu\n", total);

    Frame* frame = new Frame(id, fileName, originalImage, grayImage, gaussianImage);

    Option<Frame*> maybeFrame = Option<Frame*>(frame);
    return maybeFrame;
}

Frame* Frame::warp(Frame* frame, Box* box) {
    Frame* copied = frame->clone();
    double x1 = box->x1;
    double y1 = box->y1;

    double x2 = box->x2;
    double y2 = box->y2;
    IplImage* gaussian = copied->gaussian;

    Random::seed();
    IplImage* warpedImage = Image::warp(gaussian, x1, y1, x2, y2);
    int width = warpedImage->width;
    int height = warpedImage->height;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double p = CV_IMAGE_ELEM(warpedImage, double, i ,j);
            int pint = (int) round(p);

            int idx = ((i + y1) * gaussian->widthStep) + j + x1;
            gaussian->imageData[idx] = (uchar) pint;
        }
    }
    return copied;
}
