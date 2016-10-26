#include "core/BoundedSortedScoredBoxVector.hpp"

int main(int argc, char** args) {
    BoundedSortedScoredBoxVector vector = BoundedSortedScoredBoxVector(5);

    Box* b1 = new Box(0, 1, 1, 1, 1);
    b1->overlap = 0.9;
    ScoredBox* sb1 = new ScoredBox(b1);

    Box* b2 = new Box(1, 1, 1, 1, 1);
    b2->overlap = 0.5;
    ScoredBox* sb2 = new ScoredBox(b2);

    Box* b3 = new Box(2, 1, 1, 1, 1);
    b3->overlap = 0.7;
    ScoredBox* sb3 = new ScoredBox(b3);

    Box* b4 = new Box(3, 1, 1, 1, 1);
    b4->overlap = 0.5;
    ScoredBox* sb4 = new ScoredBox(b4);

    Box* b5 = new Box(4, 1, 1, 1, 1);
    b5->overlap = 0.1;
    ScoredBox* sb5 = new ScoredBox(b5);

    Box* b6 = new Box(5, 1, 1, 1, 1);
    b6->overlap = 0.8;
    ScoredBox* sb6 = new ScoredBox(b6);

    Box* b7 = new Box(6, 1, 1, 1, 1);
    b7->overlap = 0.6;
    ScoredBox* sb7 = new ScoredBox(b7);

    vector += sb1;
    vector += sb2;
    vector += sb3;
    vector += sb4;
    vector += sb5;
    vector += sb6;

    for (int i = 0; i < 5; i++) {
        ScoredBox* sb = vector.underlying[i];
        printf("SB(%d)\n", sb->box->id);
    }

    vector += sb7;

    for (int i = 0; i < 5; i++) {
        ScoredBox* sb = vector.underlying[i];
        printf("SB(%d)\n", sb->box->id);
    }



}
