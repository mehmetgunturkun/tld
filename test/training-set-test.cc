#include <cstdlib>
#include <cstdio>

#include "core/Box.hpp"
#include "detector/ScoredBox.hpp"
#include "detector/common/TrainingSet.hpp"

int main(int argc, char** argv) {

    vector<Box*> initialPositiveSamples;
    vector<Box*> initalNegativeSamples;
    TrainingSet<Box>* initialTs= new TrainingSet<Box>(initialPositiveSamples, initalNegativeSamples);
    printf("There are %d positive, %d negative samples in TrainingSet\n", initialTs->nrOfPositiveSamples, initialTs->nrOfNegativeSamples);

    vector<ScoredBox*> positiveSamples;
    vector<ScoredBox*> negativeSamples;
    TrainingSet<ScoredBox>* ts = new TrainingSet<ScoredBox>(positiveSamples, negativeSamples);
    printf("There are %d positive, %d negative samples in TrainingSet\n", ts->nrOfPositiveSamples, ts->nrOfNegativeSamples);

    return EXIT_SUCCESS;
}
