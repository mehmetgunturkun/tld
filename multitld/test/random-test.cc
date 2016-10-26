#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "sys/time.h"
#include <float.h>
#include <math.h>
#include "core/Random.hpp"

int main(int argc, char** argv) {

    double d1 = (double) 1 / 4;
    double d2 = (double) 1 / 1;
    double d3 = (double) 1 / 2;
    double d4 = (double) 1 / 1;
    double d5 = (double) 3 / 4;
    double d6 = (double) 1 / 3;
    double d7 = (double) 1 / 2;
    double d8 = (double) 2 / 3;
    double d9 = d1 + d2 + d3 + d4 + d5 + d6 + d7 + d8;
    printf("Sum: %g > 5.0 = %d\n", d9, d9 > 5.0);

    float f1 = (float) 1 / 4;
    float f2 = (float) 1 / 1;
    float f3 = (float) 1 / 2;
    float f4 = (float) 1 / 1;
    float f5 = (float) 3 / 4;
    float f6 = (float) 1 / 3;
    float f7 = (float) 1 / 2;
    float f8 = (float) 2 / 3;
    float f9 = f1 + f2 + f3 + f4 + f5 + f6 + f7 + f8;
    printf("Sum: %f > 5.0 = %d\n", f9, f9 > 5.0f);

    Random::seed();
    for (int i = 0; i < 10; i++) {
        float random = Random::randomFloat();
        printf("Random = %f\n", random);
    }
    return 0;
}
