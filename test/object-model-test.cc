#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "sys/time.h"
#include "core/Image.hpp"
#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/nn/Patch.hpp"
#include "detector/nn/ObjectModel.hpp"
#include "util/Dataset.hpp"

int main(int argc, char** argv) {
    Dataset* dataset = new Dataset("car");
    Frame* f = dataset->next();

    Box* b = dataset->initBox;
    Patch* sample1 = new Patch(f, b);
    Patch* sample2 = new Patch(f, b);

    ObjectModel* model = new ObjectModel();
    double similarity = model->computeSimilarity(sample1, sample2);
    println("Expected similarity score: 1.0, actual similarity score = %g", similarity);
    return EXIT_SUCCESS;
}
