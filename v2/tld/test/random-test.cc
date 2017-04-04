
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/core/types_c.h"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    int n = 10;
    CvPoint2D32f* points = (CvPoint2D32f*) malloc(sizeof(CvPoint2D32f) * n);


    for (int i = 0; i < n; i++) {
        CvPoint2D32f* p = points + i;
        p->x = i;
        p->y = i * 2;
    }

    for (int i = 0; i < n; i++) {
        CvPoint2D32f* p = points + i;
    }

    // cvFree(&points);

}
