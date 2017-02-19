// #include "core/IntegralImage.hpp"

#include <stdlib.h>
#include <stdio.h>

int* createNewObject() {
    int* arr = (int*) malloc(sizeof(int) * 10000);
    return arr;
}

int main(int argc, char** args) {
    printf("Creating objects...\n");
    for (int i = 0; i < 200000000; i++) {

        if (i % 1000) {
            printf(".");
        }

        int* arr = createNewObject();
        for (int i = 0; i < 10000; i++) {
            arr[i] = i;
        }
    }

    // IntegralImage* integralImage = new IntegralImage(mat);
    return 0;
}
