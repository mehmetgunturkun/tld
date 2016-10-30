#include "core/Image.hpp"
#include "testbase/Sequence.hpp"

int main(int argc, char** args) {
    string key(args[2]);

    printf("Going to run for %s\n", key.c_str());
    Sequence* sequence = new Sequence(key);

    if (!sequence->hasNext()) {
        fprintf(stderr, "There is no frames for %s sequence\n", "car");
        return EXIT_FAILURE;
    } else {
        fprintf(stdout, "There are %4d frames for %s sequence\n", sequence->nrOfFrames, key.c_str());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
