#include <stdlib.h>
#include <stdio.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "common/Arguments.hpp"
#include "core/Random.hpp"
#include "testbase/Sequence.hpp"

#include "detector/Detector.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979L
#endif
// rowwise access
#define coord(x, y, width, height) (x+y*width)
#define nextrow(tmp, width, height) ((tmp)+width)
#define nextcol(tmp, width, height) ((tmp)+1)
#define nextr_c(tmp, width, height) ((tmp)+width+1)

#define M(r, c) H[r*3+c]

using namespace std;
using namespace cv;

void printDoubleImage(Mat m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            double p = m.at<double>(i, j);
            printf("%8.4f ", p);
        }
        printf("\n");
    }
    printf("------------------------------\n");
}

/* Warps image of size w x h, using affine transformation matrix (2x2 part)
   and offset (center of warping) ofsx, ofsy. Result is the region of size
   defined with roi. */
void warp_image_roi_1(uchar* image, int w, int h, double *H,
                    double xmin, double xmax, double ymin, double ymax,
                    double fill, double *result) {
   double curx, cury, curz, wx, wy, wz, ox, oy, oz;
   int x, y;
   unsigned char *tmp;
   double *output=result, i, j, xx, yy;
   /* precalulate necessary constant with respect to i,j offset
      translation, H is column oriented (transposed) */
   ox = M(0,2);
   oy = M(1,2);
   oz = M(2,2);

   // printf("ox: %g, oy: %g, oz: %g\n", ox, oy, oz);

   yy = ymin;
   for (j = 0; j < (int)(ymax-ymin+1); j++)
   {
      /* calculate x, y for current row */
      curx = M(0,1)*yy + ox;
      cury = M(1,1)*yy + oy;
      curz = M(2,1)*yy + oz;
      xx = xmin;
      yy = yy + 1;
      for (i = 0; i < (int)(xmax-xmin+1); i++) {
         /* calculate x, y in current column */
         wx = M(0,0)*xx + curx;
         wy = M(1,0)*xx + cury;
         wz = M(2,0)*xx + curz;
//       printf("%g %g, %g %g %g\n", xx, yy, wx, wy, wz);
         wx /= wz;
         wy /= wz;
         xx = xx + 1;

         x = (int)floor(wx);
         y = (int)floor(wy);

         if (x>=0 && y>=0)
         {
            wx -= x; wy -= y;
            if (x+1==w && wx==1)
               x--;
            if (y+1==h && wy==1)
               y--;
            if ((x+1)<w && (y+1)<h)
            {
               tmp = &image[coord(x,y,w,h)];
               /* image[x,y]*(1-wx)*(1-wy) + image[x+1,y]*wx*(1-wy) +
                  image[x,y+1]*(1-wx)*wy + image[x+1,y+1]*wx*wy */
               *output++ =
                  (*(tmp) * (1-wx) + *nextcol(tmp, w, h) * wx) * (1-wy) +
                  (*nextrow(tmp,w,h) * (1-wx) + *nextr_c(tmp,w,h) * wx) * wy;
            } else
               *output++ = fill;
         } else
            *output++ = fill;
      }
   }
}

void warp(Mat* image) {

    // INITIALIZATION - Begin

    uchar* img = (uchar*) (malloc(sizeof(uchar) * 320 * 240));
    printf("\n-------\n");

    for (int i = 0; i < 240; i++) {
        for (int j = 0; j < 320; j++) {
            uchar p = image->at<uchar>(i, j);
            img[i * 320 + j] = p;
        }
    }

    double angle = 20.0;
    double scale = 0.02;
    double shift = 0.02;

    double x1 = 156.0;
    double y1 = 84.0;

    double x2 = 223.0;
    double y2 = 154.0;

    double width = x2 - x1;
    double height = y2 - y1;

    double meanx = (x1 + x2) / 2;
    double meany = (y1 + y2) / 2;

    double imWidth = 320;
    double imHeight = 240;

    // INITIALIZATION - End

    Mat Sh1 = Mat::zeros(3, 3, CV_64F);
    Sh1.at<double>(0, 0) = 1.0;
    Sh1.at<double>(0, 1) = 0.0;
    Sh1.at<double>(0, 2) = -meanx;

    Sh1.at<double>(1, 0) = 0.0;
    Sh1.at<double>(1, 1) = 1.0;
    Sh1.at<double>(1, 2) = -meany;

    Sh1.at<double>(2, 0) = 0.0;
    Sh1.at<double>(2, 1) = 0.0;
    Sh1.at<double>(2, 2) = 1.0;

    double scaleValue = 1 - scale * (Random::randomFloat() - 0.5);
    Mat Sca = Mat::zeros(3, 3, CV_64F);
    Sca.at<double>(0, 0) = scaleValue;
    Sca.at<double>(1, 1) = scaleValue;
    Sca.at<double>(2, 2) = 1.0;

    double angleValue = 2 * M_PI / 360 * angle * (Random::randomFloat() - 0.5);
    double ca = cos(angleValue);
    double sa = sin(angleValue);

    Mat Ang = Mat::zeros(3, 3, CV_64F);
    Ang.at<double>(0, 0) = ca;
    Ang.at<double>(0, 1) = -sa;
    Ang.at<double>(0, 2) = 0.0;

    Ang.at<double>(1, 0) = sa;
    Ang.at<double>(1, 1) = ca;
    Ang.at<double>(1, 2) = 0.0;

    Ang.at<double>(2, 0) = 0.0;
    Ang.at<double>(2, 1) = 0.0;
    Ang.at<double>(2, 2) = 1.0;

    double shR = shift * (height + 1) * (Random::randomFloat() - 0.5);
    double shC = shift * (width + 1) * (Random::randomFloat() - 0.5);

    Mat Sh2 = Mat::zeros(3, 3, CV_64F);
    Sh2.at<double>(0, 0) = 1.0;
    Sh2.at<double>(0, 1) = 0.0;
    Sh2.at<double>(0, 2) = shC;

    Sh2.at<double>(1, 0) = 0.0;
    Sh2.at<double>(1, 1) = 1.0;
    Sh2.at<double>(1, 2) = shR;

    Sh2.at<double>(2, 0) = 0.0;
    Sh2.at<double>(2, 1) = 0.0;
    Sh2.at<double>(2, 2) = 1.0;

    Mat H = Sh2 * Ang * Sca* Sh1;
    H = H.inv();

    double xmin = -(width / 2.0);
    double xmax = (width / 2.0);

    double ymin = -(height / 2.0);
    double ymax = (height / 2.0);

    double* result = new double[int (xmax - xmin + 1) * (int) (ymax - ymin + 1) ];

    printDoubleImage(H);

    double* h = (double*) H.data;

    warp_image_roi_1(img, imWidth, imHeight, h, xmin, xmax, ymin, ymax, 0.0, result);

    printf("------\n");
    for (int i = 0; i < (int) (ymax - ymin + 1); i++) {
        for (int j = 0; j < (int) (xmax - xmin + 1); j++) {
            double res = result[i * (int) (xmax - xmin + 1) + j];
            printf("%6.4f ", res);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {

    Arguments* args = new Arguments(argc, argv);
    string key = args->getString("sequence");

    Sequence* sequence = new Sequence(key);
    Frame* frame = sequence->next();
    Box* initBox = sequence->initBox;

    // println("%s", initBox->toCharArr());


    Mat* img = frame->gaussian;
    Mat* newImg = new Mat();
    img->copyTo(*newImg);

    double x1 = 156.0;
    double y1 = 84.0;

    double x2 = 223.0;
    double y2 = 154.0;

    Random::seed();
    Mat* warpedImage = Image::warp(img, x1, y1, x2, y2);
    // println("Image: %2d x %2d", warpedImage->rows, warpedImage->cols);
    //
    for (int i = 0; i < warpedImage->rows; i++) {
        for (int j = 0; j < warpedImage->cols; j++) {
            double p = warpedImage->at<double>(i, j);
            int pint = (int) round(p);
            newImg->at<uchar>(i + y1, j + x1) = (uchar) pint;
        }
    }

    // for (int i = y1 - 10; i < y2 + 10; i++) {
    //     printf("%2d >> ", i);
    //     for (int j = x1 - 2; j < x2 + 2; j++) {
    //         uchar p = newImg->at<uchar>(i, j);
    //         printf("%2d ", (int) p);
    //     }
    //
    //     printf("\n");
    // }

    Frame* warpedFrame = new Frame(frame->grayscale, newImg);
    warpedFrame->integral = frame->integral;

    vector<Box*> boxList = { initBox };
    Detector* detector = new Detector(warpedFrame, boxList);
    detector->init(warpedFrame, boxList);

    // warp(img);
    return 0;
}
