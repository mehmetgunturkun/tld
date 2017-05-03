#include "detector/ensemble/Leaf.hpp"

Leaf::Leaf() {
    nrOfNegatives = 0;
    nrOfPositives = 0;
    probability = 0.0;
}

void Leaf::incrementPositive() {
    nrOfPositives += 1;
    probability = (double) nrOfPositives / (nrOfPositives + nrOfNegatives);
}

void Leaf::incrementNegative() {
    nrOfNegatives += 1;
    probability = (double) nrOfPositives / (nrOfPositives + nrOfNegatives);
}

string Leaf::toString() {
    stringstream ss;
    ss << "Leaf("
       << "#p = " << nrOfPositives << ", "
       << "#n = " << nrOfNegatives << ")";
    return ss.str();
}
