#include "detector/ensemble/Leaf.hpp"

Leaf::Leaf() {
    nrOfNegatives = 0;
    nrOfPositives = 0;
    probability = 0.0;
}

void Leaf::incrementPositive() {
    nrOfPositives += 1;
    probability = nrOfPositives / (nrOfPositives + nrOfNegatives);
}

void Leaf::incrementNegative() {
    nrOfNegatives += 1;
    probability = nrOfPositives / (nrOfPositives + nrOfNegatives);
}
