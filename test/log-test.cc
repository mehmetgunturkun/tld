#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "sys/time.h"
#include "util/Log.hpp"

int main(int argc, char** argv) {
    Log::init();
    Log::startTimer("test1M");
    int total = 0;
    for (long i = 0; i < 1000000000l; i++) {
        total += 1;
    }
    double period = Log::stopTimer("test1M");
    printf("Period => %g\n", period);

    Log::startTimer("test9M");
    for (long i = 0; i < 9000000000l; i++) {
        total += 1;
    }
    period = Log::stopTimer("test9M");
    printf("Period => %g\n", period);

    Log::startTimer("test100M");
    for (long i = 0; i < 100000000000l; i++) {
        total += 1;
    }
    period = Log::stopTimer("test100M");
    printf("Period => %g\n", period);

    Log::finalize();
    return 0;
}
