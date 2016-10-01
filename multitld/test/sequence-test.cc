#include "core/Image.hpp"
#include "testbase/Sequence.hpp"

int main(int argc, char** args) {
    Sequence* sequence = new Sequence("car");

    float t1 = 120.231;
    double t2 = 120.231;

    printf("%f, %g\n", t1, t2);

    if (!sequence->hasNext()) {
        fprintf(stderr, "There is no frames for %s sequence\n", "car");
        return EXIT_FAILURE;
    }

    while (sequence->hasNext()) {
        Frame* current = sequence->next();
    }
    return EXIT_SUCCESS;
}
