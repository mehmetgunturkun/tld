#include "testbase/Sequence.hpp"
#include "core/BoxIterator.hpp"
#include "util/ImageBuilder.hpp"

//
// double roundDouble(double n) {
//     int intN = (int) n;
//     double precision = n - intN;
//     if (precision >= 0.49) {
//         return (double) intN + 1;
//     } else {
//         return (double) intN;
//     }
// }

// class BoxTemplate {
// public:
//     double x1;
//     double y1;
//
//     double width;
//     double height;
//
//     double stepX;
//     double stepY;
//
//     double limitX;
//     double limitY;
//
//     bool preStart;
//     int nrOfBoxes;
//
//     BoxTemplate(double boxWidth, double boxHeight, double frameWidth, double frameHeight) {
//         this->x1 = 1.0;
//         this->y1 = 1.0;
//
//         this->preStart = true;
//         this->nrOfBoxes = 0;
//
//         this->width = boxWidth;
//         this->height = boxHeight;
//
//         this->stepX = round(this->width) * 0.1;
//         this->stepY = round(this->height) * 0.1;
//
//         this->stepX = min(this->stepX, this->stepY);
//         this->stepY = min(this->stepX, this->stepY);
//
//         this->limitX = roundDouble(frameWidth - this->width - 1);
//         this->limitY = roundDouble(frameHeight - this->height - 1);
//     }
//
//     bool hasNext() {
//         if (this->preStart) {
//             this->preStart = false;
//             return true;
//         } else {
//             double nextX1 = this->x1 + stepX;
//             if (roundDouble(nextX1) >= limitX) {
//                 double nextY1 = this->y1 + stepY;
//                 if (roundDouble(nextY1) >= limitY) {
//                     printf("%d boxes\n", nrOfBoxes);
//                     return false;
//                 }
//                 this->x1 = 1.0;
//                 this->y1 = nextY1;
//             } else {
//                 this->x1 = nextX1;
//             }
//             return true;
//         }
//     }
//
//     Box* next() {
//         nrOfBoxes += 1;
//         Box* box = new Box(0, roundDouble(this->x1), roundDouble(this->y1), roundDouble(this->x1) + this->width - 1, roundDouble(this->y1) + this->height - 1);
//         return box;
//     }
// };
//
// class BoxTracer {
// public:
//     Frame* frame;
//     vector<BoxTemplate*> boxPerScale;
//     int scaleNo;
//     int nrOfTotalScales;
//     int maxScale;
//
//     int boxCounter;
//
//     BoxTracer(Frame* f, Box* box) {
//         this->frame = f;
//         boxCounter = 0;
//         this->scaleNo = 0;
//         this->maxScale = 10;
//         this->nrOfTotalScales = 0;
//
//         initBoxesPerScale(box);
//     }
//
//     void initBoxesPerScale(Box* box) {
//         double width = (double) box->width;
//         double height = (double) box->height;
//
//         double scaleFactor = 1.2;
//         for (int i = -10; i <= 10; i++) {
//             double scale = pow(scaleFactor, i);
//             double scaledWidth = roundDouble(((double) width) * scale);
//             double scaledHeight = roundDouble(((double) height) * scale);
//             if (scaledWidth > 24.0 && scaledHeight > 24.0  && scaledWidth < this->frame->width && scaledHeight < this->frame->height) {
//                 this->nrOfTotalScales += 1;
//                 BoxTemplate* boxTemplate = new BoxTemplate(scaledWidth, scaledHeight, this->frame->width, this->frame->height);
//                 boxPerScale.push_back(boxTemplate);
//             }
//         }
//
//         printf("There are %d templates\n", (int) boxPerScale.size());
//     }
//
//     bool hasNext() {
//         if (this->scaleNo < maxScale && this->scaleNo < nrOfTotalScales) {
//             BoxTemplate* boxTemplate = boxPerScale[this->scaleNo];
//             bool hasMoreTemplate = boxTemplate->hasNext();
//             if (hasMoreTemplate) {
//                 return true;
//             } else {
//                 this->scaleNo += 1;
//                 return hasNext();
//             }
//         } else {
//             return false;
//         }
//     }

    // bool hasNext() {
    //
    //     while (boxTemplate->width < 24 || boxTemplate->height < 24 || (this->scaleNo < maxScale && this->scaleNo > nrOfTotalScales)) {
    //
    //         this->scaleNo = this->scaleNo + 1;
    //         boxTemplate = boxPerScale[this->scaleNo];
    //     }
    //
    //     return (this->scaleNo < maxScale && this->scaleNo < nrOfTotalScales);
    // }
    //
    // void scaleUp() {
    //     this->scaleNo = this->scaleNo + 1;
    // }
    //
    // Box* next() {
    //     BoxTemplate* boxTemplate = boxPerScale[this->scaleNo];
    //     return boxTemplate->next();
    // }

    // Box* createInitBox(Box* b) {
    //     double nextWidth = b->width / 1.2;
    //     double nextHeight = b->height / 1.2;
    //
    //     if (b->scaleNo >= 10) {
    //         return box;
    //     } else if (nextWidth < 24 || nextHeight < 24) {
    //         printf("INIT >>> %g x %g\n",  roundDouble(b->width), roundDouble(b->height));
    //         return box;
    //     } else {
    //         b->scaleNo = b->scaleNo + 1;
    //         printf("NEXT >>> %g x %g\n",  roundDouble(b->width), roundDouble(b->height));
    //         b->width = nextWidth;
    //         b->height = nextHeight;
    //         return createInitBox(b);
    //     }
    // }
    //
    // void setSteps() {
    //     stepX = round(this->box->width) * 0.1;
    //     printf("%f, %f\n", this->box->width, stepX);
    //     stepY = round(this->box->height) * 0.1;
    //
    //     stepX = min(stepX, stepY);
    //     stepY = min(stepX, stepY);
    // }
    //
    // void setLimits() {
    //     limitX = ((double) this->frame->width) - this->width - 1;
    //     limitY = ((double) this->frame->height) - this->height - 1;
    // }



    // void scaleUp() {
    //     // this->maxScale = maxScale - 1;
    //
    //     this->width = roundDouble(this->width * 1.2);
    //     this->height = roundDouble(this->height * 1.2);
    //
    //     printf("SCALE UP >>> %g x %g\n",  this->width, this->height);
    //
    //     this->x1 = 1.0;
    //     this->y1 = 1.0;
    //
    //     setSteps();
    //     setLimits();
    //     // levelBasedCount = 0;
    // }
    //
    // bool hasNext() {
    //     if (preStart) {
    //         preStart = false;
    //         return true;
    //     } else {
    //         double nextX1 = this->x1 + stepX;
    //         if (roundDouble(nextX1) >= roundDouble(limitX)) {
    //             double nextY1 = this->y1 + stepY;
    //             if (roundDouble(nextY1) >= roundDouble(limitY)) {
    //                 // This level is done!
    //                 // printf("Y exceeded - %g\n", limitY);
    //                 // printf("Level is done!\n");
    //
    //                 scaleUp();
    //
    //                 if (this->width > frame->width || this->height > frame->height) {
    //                     return false;
    //                 } else {
    //                     return true;
    //                 }
    //             }
    //             this->x1 = 1.0;
    //             this->y1 = nextY1;
    //         } else {
    //             this->x1 = nextX1;
    //         }
    //         return true;
    //     }
    // }
    //
    // Box* next() {
    //     int id = boxCounter;
    //     boxCounter += 1;
    //     Box* scaledBox = boxPerScale[scaleNo];
    //
    //     double roundedX1 = roundDouble(this->x1);
    //     double roundedY1 = roundDouble(this->y1);
    //     double roundedWidth = roundDouble(scaledBox->width);
    //     double roundedHeight = roundDouble(scaledBox->height);
    //
    //     Box* nextBox = new Box(id,
    //         roundedX1,
    //         roundedY1,
    //         roundedX1 + roundedWidth - 1.0,
    //         roundedY1 + roundedHeight - 1.0);
    //     return nextBox;
    // }
// };



int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("tld/car");
    Frame* firstFrame = sequence->next();
    Box* firstBox = sequence->initBox;

    BoxIterator* boxTracer = new BoxIterator(firstFrame, firstBox, 10, 24);
    while (boxTracer->hasNext()) {
        Box* b = boxTracer->next();
        // printf("%s\n", b->toCharArr());
    }


    // while (boxTracer->hasNext()) {
    //     n += 1;
    //     Box* b = boxTracer->next();
    //     printf("%s\n", b->toCharArr());
    // }



    // printf("Frame (%d x %d)\n", firstFrame->width, firstFrame->height);
    //
    // int nrOfTotalBoxes = 0;
    // float width = 0;
    // float height = 0;
    // int scaleNo = 0;

    // printf("%s\n", b->toCharArr());
    // while (boxIterator->hasNext()) {
        // Box* sampleBox = boxIterator->next();
        // if (width != sampleBox->width || height != sampleBox->height) {
        //     printf("%5d Total Boxes for Scale: %f x %f\n", nrOfTotalBoxes, width, height);
        //     width = sampleBox->width;
        //     height = sampleBox->height;
        //     nrOfTotalBoxes = 0;
        //     scaleNo += 1;
        // }

        // printf("%s\n", sampleBox->toCharArr());
        //
        // nrOfTotalBoxes += 1;
    // }
}
