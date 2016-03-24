#include <cstdlib>
#include <cstdio>

#include "core/Box.hpp"
#include "detector/ScoredBox.hpp"
#include "detector/common/TrainingSet.hpp"

#include "common/BoundedPriorityQueue.hpp"
#include "util/Util.h"


int main(int argc, char** argv) {
    // std::priority_queue<int> queue;
    // queue.comp(3,4);

    println("Hello World!");

    BoundedPriorityQueue<Box, OverlapOrdered> queue = BoundedPriorityQueue<Box, OverlapOrdered>(3);
    Box* b1 = new Box();
    b1->overlap = 0.9;

    Box* b2 = new Box();
    b2->overlap = 0.1;

    queue += b1;
    queue += b2;

    Frame* frame = new Frame("resources/test.jpg");

    vector<Box*> initialPositiveSamples;
    vector<Box*> initalNegativeSamples;
    TrainingSet<Box>* initialTs= new TrainingSet<Box>(frame, initialPositiveSamples, initalNegativeSamples);
    printf("There are %d positive, %d negative samples in TrainingSet\n", initialTs->nrOfPositiveSamples, initialTs->nrOfNegativeSamples);

    vector<ScoredBox*> positiveSamples;
    vector<ScoredBox*> negativeSamples;
    TrainingSet<ScoredBox>* ts = new TrainingSet<ScoredBox>(frame, positiveSamples, negativeSamples);
    printf("There are %d positive, %d negative samples in TrainingSet\n", ts->nrOfPositiveSamples, ts->nrOfNegativeSamples);

    return EXIT_SUCCESS;
}
