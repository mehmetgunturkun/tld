#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "sys/time.h"
#include "core/Random.hpp"

int main(int argc, char** argv) {
    Random::seed();

    for (int i = 0; i < 10; i++) {
        float random = Random::randomFloat();
        printf("Random = %5.4f\n", random);
    }
    return 0;
}
