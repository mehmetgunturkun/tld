#include <cstdlib>
#include <cstdio>

#include "common/Option.hpp"

int main(int argc, char** argv) {
    Option<int>* maybeInt = new None<int>();

    int* item = maybeInt->getOrElse(10);
    printf("Item: %d\n", *item);
    return EXIT_SUCCESS;
}
