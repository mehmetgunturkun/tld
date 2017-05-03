#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/IntegralImage.hpp"
#include "core/Sequence.hpp"

int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);
    string sequenceKey = args->getString("sequence");

    for (int i = 0; i < 1; i++) {
        println("Test.%d is running", i);
        Sequence sequence = Sequence(sequenceKey);
        Box* initBox = sequence.initBox;
        if (sequence.hasNext()) {
           Frame* currentFrame = sequence.next();
           IntegralImage* integralImage = new IntegralImage(currentFrame->grayscale);

           MeanVariance* meanVariance = integralImage->computeMeanVariance(
               (int) initBox->x1,
               (int) initBox->y1,
               (int) initBox->width,
               (int) initBox->height
           );

           println("Mean: %10.5f, Variance: %10.5f", meanVariance->mean, meanVariance->variance);

           //delete integralImage;
           delete currentFrame;
       } else {
           println("There is no frame in this sequence");
       }
   }
}
