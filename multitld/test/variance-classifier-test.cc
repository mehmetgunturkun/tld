
#include "detector/variance/VarianceClassifier.hpp"
#include "testbase/Sequence.hpp"

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("car");

    Frame* frame = sequence->next();

    Box* initBox = sequence->initBox;

    Box* b0 = initBox->move(0.0f, 0.0f, 0.0f, 0.0f);
    Box* b1 = initBox->move(50.0f, 0.0f, 50.0f, 0.0f);
    Box* b2 = initBox->move(-50.0f, 0.0f, 50.0f, 0.0f);
    Box* b3 = initBox->move(50.0f, 0.0f, -50.0f, 0.0f);
    Box* b4 = initBox->move(-50.0f, 0.0f, -50.0f, 0.0f);


    vector<Box*> boxList = { b0, b1, b2, b3, b4 };
    VarianceClassifier* varianceClassifier = new VarianceClassifier();
    varianceClassifier->init(frame, boxList);
    printf("Minimum Variance Value: %f\n", varianceClassifier->minimumVariance);


    return EXIT_SUCCESS;
}
