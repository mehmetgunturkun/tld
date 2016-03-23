#include "detector/nn/Patch.hpp"

int Patch::WODTH = 15;
int Patch::HEIGHT = 15;

Patch::Patch() {
    data = (int*)malloc(sizeof(int) * Patch::WODTH * Patch::HEIGHT);
}

Patch::Patch(int* d) {
    data = d;
}

Patch::Patch(Frame* frame, Box* box) {
    
}
