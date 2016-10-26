#include <stdlib.h>
#include <stdio.h>

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "testbase/Sequence.hpp"

// void createPatch(Frame* f, Box* b) {
//     double centerX = (box->x1 + box->x2) / 2.0;
//     double centerY = (box->y1 + box->y2) / 2.0;
//
//     double width = b->width;
//     double height = b->height;
//
//
// }

#ifndef M_PI
#define M_PI 3.14159265358979L
#endif
// rowwise access
#define coord(x, y, width, height) (x+y*width)
#define nextrow(tmp, width, height) ((tmp)+width)
#define nextcol(tmp, width, height) ((tmp)+1)
#define nextr_c(tmp, width, height) ((tmp)+width+1)

#define M(r, c) H[r*3+c]

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

int main(int argc, char** args) {
    Sequence* sequence = new Sequence("tld/car");

    Frame* firstFrame = sequence->next();
    Frame* secondFrame = sequence->next();

    Box* box = new Box(0, 137.228, 124.577, 227.049, 163.499);


    uchar* img = secondFrame->grayscale->data;
    int width = secondFrame->width;
    int height = secondFrame->height;

    double xmin = -(box->width / 2.0);
    double xmax = (box->width / 2.0);

    double ymin = -(box->height / 2.0);
    double ymax = (box->height / 2.0);

    double* H = (double*) malloc(sizeof(double) * 9);
    H[0] = 1.0;
    H[1] = 0.0;
    H[2] = 182.1386;
    H[3] = 0.0;
    H[4] = 1.0;
    H[5] = 144.0377;
    H[6] = 0.0;
    H[7] = 0.0;
    H[8] = 1.0;

    int size = ((int)(xmax-xmin+1)*(int)(ymax-ymin+1));
    int w = (int) (xmax-xmin+1);
    int h = (int) (ymax-ymin+1);

    printf("%d, %d\n", w, h);
    double* result = new double[size];



    warp_image_roi(img, width, height, H, xmin, xmax, ymin, ymax, 0.0, result);

    Mat* matrix = new Mat(h, w, CV_8U, 0.0);
    printf("%d \n", (int) matrix->at<uchar>(0, 0));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int myPixel = (int) matrix->at<uchar>(i, j);
            printf("%d, %d, %d\n", i, j, myPixel);
            matrix->at<uchar>(i, j) = (int) round(result[i * w + j]);
            myPixel = (int) matrix->at<uchar>(i, j);
            printf("%d, %d, %d\n", i, j, myPixel);

        }
        printf("\n--------------\n");
    }

}
