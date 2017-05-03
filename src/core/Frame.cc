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

    //TODO Check the true way to release gaussian image.
    // printf("mc1200\n");
    // cout << gaussian << endl;
    // cvReleaseImage(&gaussian);

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

    IplImage* gaussianImage;
    if (originalImage->nChannels == 3) {
        IplImage* gray4Blur = Image::bgr2gray(originalImage);
        long total3 = 0;
        for (int i = 0; i < gray4Blur->height; i++) {
            for (int j = 0; j < gray4Blur->width; j++) {
                uchar pixel = CV_IMAGE_ELEM(gray4Blur, uchar, i, j);
                total3 += pixel;
            }
        }

        gaussianImage = Gaussian::blur(gray4Blur, 12, 2.0);
        cvReleaseImage(&gray4Blur);

    } else {
        IplImage* gray4Blur = Image::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
        gaussianImage = Gaussian::blur(gray4Blur, 12, 2.0);
        cvReleaseImage(&gray4Blur);
    }

    total = 0;
    for (int i = 0; i < grayImage->height; i++) {
        for (int j = 0; j < grayImage->width; j++) {
            uchar pixel = CV_IMAGE_ELEM(gaussianImage, uchar, i, j);
            total += pixel;
        }
    }

    Frame* frame = new Frame(id, fileName, originalImage, grayImage, gaussianImage);

    Option<Frame*> maybeFrame = Option<Frame*>(frame);
    return maybeFrame;
}

Frame* Frame::warp(Frame* frame,
    Box* box,
    double angle,
    double scale,
    double shift) {
        Frame* copied = frame->clone();
        double x1 = box->x1;
        double y1 = box->y1;

        double x2 = box->x2;
        double y2 = box->y2;
        IplImage* gaussian = copied->gaussian;

        Random::seed();
        IplImage* warpedImage = Image::warp(gaussian, x1, y1, x2, y2, angle, scale, shift);
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
        cvReleaseImage(&warpedImage);
        return copied;
}
