#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/IntegralImage.hpp"
#include "core/Sequence.hpp"

int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);
    string sequenceKey = args->getString("sequence");

    for (int i = 0; i < 100; i++) {
        println("Test.%d is running", i);
        Sequence sequence = Sequence(sequenceKey);

        if (sequence.hasNext()) {
           Frame* currentFrame = sequence.next();
           //IntegralImage* integralImage = new IntegralImage(currentFrame->grayscale);

           //delete integralImage;
           delete currentFrame;
       } else {
           println("There is no frame in this sequence");
       }
   }
}
