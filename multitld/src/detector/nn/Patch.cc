#include "detector/nn/Patch.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979L
#endif
// rowwise access
#define coord(x, y, width, height) (x+y*width)
#define nextrow(tmp, width, height) ((tmp)+width)
#define nextcol(tmp, width, height) ((tmp)+1)
#define nextr_c(tmp, width, height) ((tmp)+width+1)

#define M(r, c) H[r*3+c]

int Patch::WIDTH = 15;
int Patch::HEIGHT = 15;

Patch::Patch(int id, Mat* d) {
    this->id = id;
    data = d;
}

Rect* toRect(Frame* frame, Box* box) {
    double left = 0.0;
    if (box->x1 <= 0) {
        left = 0.0;
    } else {
        left = box->x1;
    }

    double right = 0.0;
    if (box->x2 > frame->width) {
        right = frame->width;
    } else {
        right = box->x2;
    }

    double top = 0.0;
    if (box->y1 <= 0) {
        top = 0.0;
    } else {
        top = box->y1;
    }

    double bottom = 0.0;
    if (box->y2 > frame->height) {
        bottom = frame->height;
    } else {
        bottom = box->y2;
    }

    Rect* rect = new Rect(left, top, right - left + 1, bottom - top + 1);
    return rect;
}

Mat* convertToDouble(Mat* m) {
    int height = (int) m->rows;
    int width = (int) m->cols;

    Mat* src = new Mat(height, width, CV_64F, 0.0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel = (int) m->at<uchar>(i, j);
            // printf("%d ", pixel);
            src->at<double>(i, j) = (double) pixel;
        }
        // printf("\n");
    }
    return src;
}

double computeMean(Mat* m) {
    int height = m->rows;
    int width = m->cols;
    int nrOfPixels = height * width;

    double sum = 0.0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double pixel = m->at<double>(i, j);
            double sum1 = sum + pixel;
            sum = sum1;
        }
    }

    double mean = sum / nrOfPixels;
    return mean;
}

Mat* constructStandardPatch(Frame* frame, Box* box) {
    //TODO Review if we can do with memcpy?
    Rect* rect = toRect(frame, box);
    Mat* patch = convertToDouble(new Mat(*(frame->grayscale), *rect));

    Mat* normalizedPatch = new Mat(15, 15, CV_64F, 0.0);
    cv::resize(*patch, *normalizedPatch, Size(15, 15), INTER_LINEAR);

    double mean = computeMean(normalizedPatch);
    for (int i = 0 ; i < Patch::HEIGHT; i++) {
        for (int j = 0 ; j < Patch::WIDTH; j++) {
            normalizedPatch->at<double>(i, j) = normalizedPatch->at<double>(i, j) - mean;
        }
    }
    return normalizedPatch;
}


/* Warps image of size w x h, using affine transformation matrix (2x2 part)
   and offset (center of warping) ofsx, ofsy. Result is the region of size
   defined with roi. */
void warp_image_roi(uchar* image, int w, int h, double *H,
                    double xmin, double xmax, double ymin, double ymax,
                    double fill, double *result)
{
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

Mat* constructSubPixelPatch(Frame* frame, Box* box) {
    uchar* img = frame->grayscale->data;
    int width = frame->width;
    int height = frame->height;

    double xmin = -(box->width / 2.0);
    double xmax = (box->width / 2.0);

    double ymin = -(box->height / 2.0);
    double ymax = (box->height / 2.0);

    double* H = (double*) malloc(sizeof(double) * 9);
    H[0] = 1.0;
    H[1] = 0.0;
    H[2] = (box->x1 + box->x2) / 2.0;
    H[3] = 0.0;
    H[4] = 1.0;
    H[5] = (box->y1 + box->y2) / 2.0;
    H[6] = 0.0;
    H[7] = 0.0;
    H[8] = 1.0;

    int w = (int)(xmax-xmin+1);
    int h = (int)(ymax-ymin+1);
    int size = ((int)(xmax-xmin+1)*(int)(ymax-ymin+1));
    double* result = new double[size];
    Mat* m = new Mat(h, w, CV_8U, 0.0);
    uchar* normalizedPatchData = m->data;

    warp_image_roi(img, width, height, H, xmin, xmax, ymin, ymax, 0.0, result);
    for (int i = 0; i < size; i++) {
        normalizedPatchData[i] = (int) round(result[i]);
    }
    Mat* patch = convertToDouble(m);

    Mat* normalizedPatch = new Mat(15, 15, CV_64F, 0.0);
    cv::resize(*patch, *normalizedPatch, Size(15, 15), INTER_LINEAR);

    double mean = computeMean(normalizedPatch);
    for (int i = 0 ; i < Patch::HEIGHT; i++) {
        for (int j = 0 ; j < Patch::WIDTH; j++) {
            normalizedPatch->at<double>(i, j) = normalizedPatch->at<double>(i, j) - mean;
        }
    }
    return normalizedPatch;
}

Patch::Patch(Frame* frame, Box* box) {
    this->id = box->id;
    if (box->isStandard()) {
        data = constructStandardPatch(frame, box);
    } else {
        data = constructSubPixelPatch(frame, box);
    }
}
