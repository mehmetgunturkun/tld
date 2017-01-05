#include <stdlib.h>
#include <stdio.h>

#define println(format, ...) printf(format, ##__VA_ARGS__); printf("\n");

class FatObject {
    char* arr;
public:
    FatObject() {
        //10KB
        arr = (char*) malloc(sizeof(char) * 10000);
    }
};

void createNewObject() {
    char* arr = (char*) malloc(sizeof(char) * 10000);
}

int main(int argc, char** args) {
    for (int i = 0; i < 200000000; i++) {
        createNewObject();
    }
    println("Done\n");
}
