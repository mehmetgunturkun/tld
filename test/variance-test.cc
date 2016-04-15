#include <cstdlib>
#include <cstdio>
#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/common/BoxIterator.hpp"
#include "detector/nn/Patch.hpp"
#include "util/Util.h"
#include "util/Dataset.hpp"

int main(int argc, char** argv) {
    Dataset* dataset = new Dataset("car");
    Frame* f = dataset->next();
    println(f->toString().c_str());

    Box* b = dataset->initBox;
    println(b->toString().c_str());

    BoxIterator* iterator = new BoxIterator(f, b, 10, 24);

    while (iterator->hasNext()) {
        Scalar mean, stddev;
        Box* nextBox = iterator->next();

        MeanVariance* stats = f->integral->computeMeanVariance(
            (int) nextBox->x1,
            (int) nextBox->y1,
            (int) nextBox->width,
            (int) nextBox->height);

        println("[VAR] = %f", stats->variance);
    }
    return EXIT_SUCCESS;
}
