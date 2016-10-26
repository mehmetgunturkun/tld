#include "testbase/Sequence.hpp"
#include "detector/ensemble/EnsembleClassifier.hpp"


void createPoints(Box* box) {
    
}

int main(int argc, char** argv) {

    Sequence* sequence = new Sequence("tld/car");
    Frame* firstFrame = sequence->next();
    Box* firstBox = sequence->initBox;

    EnsembleClassifier* eClassifer = new EnsembleClassifier();
    eClassifer->init(firstFrame, {firstBox} );

    int nrOfBaseClassifiers = eClassifer->nrOfBaseClassifiers;
    vector<BaseClassifier*> classifiers = eClassifer->baseClassifiers;
    // Random::seed();
    // float rand = Random::randomFloat();
    // printf("%f\n", rand);
    //
    // rand = Random::randomFloat();
    // printf("%f\n", rand);
    //
    // rand = Random::randomFloat();
    // printf("%f\n", rand);

    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = classifiers[i];
        bc->printPixelComparison();
    }

    return 1;
}
