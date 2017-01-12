#ifndef WARP_H
#define WARP_H

#ifndef M_PI
#define M_PI 3.14159265358979L
#endif
// rowwise access
#define coord(x, y, width, height) (x+y*width)
#define nextrow(tmp, width, height) ((tmp)+width)
#define nextcol(tmp, width, height) ((tmp)+1)
#define nextr_c(tmp, width, height) ((tmp)+width+1)

#define M(r, c) H[r*3+c]

using namespace cv;

class Warp {
private:
    /* Warps image of size w x h, using affine transformation matrix (2x2 part)
       and offset (center of warping) ofsx, ofsy. Result is the region of size
       defined with roi. */
    static void warp_image_roi(uchar* image, int w, int h, double *H,
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

public:
    static Mat* warp(Mat* img, double* h, double xMin, double yMin, double xMax, double yMax) {
        int imageWidth = img->cols;
        int imageHeight = img->rows;

        uchar* imageData = (uchar*) (malloc(sizeof(uchar) * imageWidth * imageHeight));
        for (int i = 0; i < imageHeight; i++) {
            for (int j = 0; j < imageWidth; j++) {
                uchar p = img->at<uchar>(i, j);
                imageData[i * imageWidth + j] = p;
            }
        }

        int width = (int) (xMax - xMin + 1);
        int height = (int) (yMax - yMin + 1);

        Mat* result = new Mat(height, width, CV_64F, 0.0);
        double* resultData = (double*) result->data;


        warp_image_roi(imageData, imageWidth, imageHeight, h, xMin, xMax, yMin, yMax, 0.0, resultData);

        return result;
    }
};
#endif
