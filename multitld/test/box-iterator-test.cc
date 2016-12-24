#include "testbase/Sequence.hpp"
#include "common/Colors.hpp"
#include "core/BoxIterator.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include <iostream>
#include <string>
#include <sstream>

#include <vector>

using namespace std;


int main(int argc, char** argv) {
    vector<string> dataset = {
        "01_david",
        "02_jumping",
        "03_pedestrian1",
        "04_pedestrian2",
        "05_pedestrian3",
        "06_car",
        "07_motocross",
        "08_volkswagen",
        "09_carchase",
        "10_panda"
    };

    vector<int> gridSize = {
        26574,
        31231,
        20486,
        12092,
        10789,
        18130,
        31823,
        81963,
        18621,
        31600
    };

    int nrOfDataset = (int) dataset.size();
    printf("There are %d sequences in dataset\n", nrOfDataset);
    for (int i = 0; i < nrOfDataset; i++) {
        string key = dataset[i];
        int size = gridSize[i];

        Sequence* sequence = new Sequence(key);
        Frame* firstFrame = sequence->next();
        Box* firstBox = sequence->initBox;

        BoxIterator* iterator = new BoxIterator(firstFrame, firstBox);
        while (iterator->hasNext()) {
            Box* b = iterator->next();
            free(b);
        }

        int nrOfBoxes = iterator->nrOfBoxes;
        if (nrOfBoxes == size) {
            printf(GREEN("Seq(%s) => total number of boxes: %d = %d\n"), key.c_str(), nrOfBoxes, size);
        } else {
            printf(RED("Seq(%s) => total number of boxes: %d ≠ %d\n"), key.c_str(), nrOfBoxes, size);
        }

        free(iterator);
    }
    return 0;
}
