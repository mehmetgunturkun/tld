#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "sys/time.h"
#include "util/Random.hpp"

int main(int argc, char** argv) {
    Random::seed();

    float random = Random::randomFloat();
    printf("Random = %f\n", random);
    return 0;
}
