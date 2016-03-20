#include "util/Random.hpp"

int Random::mti = RAND_N+1;

unsigned long* Random::mt = (unsigned long*)malloc(sizeof(unsigned long) * RAND_N);
