
#include "testbase/Sequence.hpp"
#include "detector/nn/Patch.hpp"
#include "detector/nn/ObjectModel.hpp"

void printDoubleImage(Mat* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            double p = m->at<double>(i, j);
            printf("% 02.6g ", p);
        }
        printf("\n------------------------------------------------\n");
    }
}

Rect* createRect(Frame* frame, Box* box) {
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
//
// void createPatch(Frame* f, Box* b) {
//     Rect* rect = toRect(f, b);
//     Mat patch = Mat(*(f->grayscale), *rect);
//     int height = (int) b->height;
//     int width = (int) b->width;
//
//     printf("Size(%d, %d)\n", width, height);
//
//     for (int i = 0; i < height; i++) {
//         printf("Line#%02d >> ", i);
//         for (int j = 0; j < width; j++) {
//             int pixel = (int) patch.at<uchar>(i, j);
//             printf("%d ", pixel);
//         }
//         printf("\n------------------------------------------\n");
//     }
//
//     Mat src = Mat(height, width, CV_64F, 0.0);
//     for (int i = 0; i < height; i++) {
//         for (int j = 0; j < width; j++) {
//             int pixel = (int) patch.at<uchar>(i, j);
//             src.at<double>(i, j) = (double) pixel;
//         }
//     }
//
//     Mat normalizedPatch = Mat(15, 15, CV_64F, 0.0);
//     cv::resize(src, normalizedPatch, Size(15, 15), INTER_LINEAR);
//
//     for (int i = 0; i < 15; i++) {
//         printf("Line#%02d >> ", i);
//         for (int j = 0; j < 15; j++) {
//             double pixel = normalizedPatch.at<double>(i, j);
//             int pixelInt = (int) ceil(pixel);
//             printf("%d ", pixelInt);
//         }
//         printf("\n------------------------------------------\n");
//     }

    // Mat normalizedPatch = Mat(15, 15, CV_32F, 0.0);
    // cv::resize(patch, normalizedPatch, normalizedPatch.size(), INTER_LINEAR);
    //
    // for (int i = 0; i < 15; i++) {
    //     for (int j = 0; j < 15; j++) {
    //         float pixel = normalizedPatch.at<float>(i, j);
    //         printf("%f ", pixel);
    //     }
    //     printf("\n");
    // }

    // Mat* normalizedTmpPatch = new Mat(Patch::HEIGHT, Patch::WIDTH, CV_8U, 0.0);
    // Mat* normalizedPatch = new Mat(Patch::HEIGHT, Patch::WIDTH, CV_32F, 0.0);
    // cv::resize(*patch, *normalizedTmpPatch, normalizedPatch->size(), INTER_LINEAR);
    //
    // for (int i = 0; i < 15; i++) {
    //     for (int j = 0; j < 15; j++) {
    //
    //     }
    // }

// }

Mat* createFloatMat(Mat* m) {
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

double computeVariance(Patch* patch) {
    vector<double> pixels;
    for (int i = 0; i < Patch::HEIGHT; i++) {
        for (int j = 0; j < Patch::WIDTH; j++) {
            double value = patch->data->at<double>(i,j);
            pixels.push_back(value);
        }
    }

    int nrOfPixels = (int) pixels.size();

    double sum = 0.0;
    for (int i = 0; i < nrOfPixels; i++) {
        sum += pixels[i];
    }
    double mean = sum / nrOfPixels;
    double variance = 0.0;
    for (int i = 0; i < nrOfPixels; i++) {
        variance += pow(pixels[i] - mean, 2);
    }

    variance = variance / nrOfPixels;
    return variance;
}

void createPatch(Frame* frame, Box* box) {
    //TODO Review if we can do with memcpy?
    Rect* rect = createRect(frame, box);
    Mat* patch = new Mat(*(createFloatMat(frame->grayscale)), *rect);

    double sum = 0.0;

    for (int i = 0 ; i < patch->rows; i++) {
        for (int j = 0 ; j < patch->cols; j++) {
            double p = patch->at<double>(i, j);
            sum += p;
            printf("%3.3f ", p);
        }
        printf("\n");
    }

    printf("SUM >>> %3.3f\n", sum);

    Mat* normalizedPatch = new Mat(15, 15, CV_64F, 0.0);
    cv::resize(*patch, *normalizedPatch, Size(15, 15), INTER_CUBIC);

    sum = 0.0;
    for (int i = 0 ; i < Patch::HEIGHT; i++) {
        for (int j = 0 ; j < Patch::WIDTH; j++) {
            double p = normalizedPatch->at<double>(i, j);
            sum += p;
            printf("%3.3f ", p);
        }
        printf("\n");
    }

    printf("SUM >>> %3.3f\n", sum);
    double mean = sum / 225;
    printf("MEAN >>> %3.3f\n", mean);

    double variance = 0.0;
    for (int i = 0 ; i < Patch::HEIGHT; i++) {
        for (int j = 0 ; j < Patch::WIDTH; j++) {
            double p = normalizedPatch->at<double>(i, j);
            p = p - mean;
            variance += (p * p);
        }
    }

    variance = variance / 224;
    printf("VAR >>> %3.3f\n", variance);
}

int main() {
    Sequence* sequence = new Sequence("tld/car");

    Frame* f0 = sequence->next();
    Box* b0 = new Box(0, 141, 125, 231, 164);
    createPatch(f0, b0);
    // Patch* p0 = new Patch(f0, b0);

    // Frame* f1 = sequence->next();
    // Box* b1 = new Box(13157, 133, 121, 223, 160);
    // Patch* p1 = new Patch(f1, b1);
    //
    // Mat* m = p1->data;
    // for (int i = 0; i < 15; i++) {
    //     for (int j = 0; j < 15; j++) {
    //         double pixel = m->at<double>(i, j);
    //         printf("%g ", pixel);
    //     }
    //     printf("\n");
    // }


    // ObjectModel* model = new ObjectModel();
    // model->add(p0, true);

    // float score = model->computePositiveSimilarity(p1);
    // printf("Positive Score: %f\n", 1 - score);

}
