#ifndef LEAF_H
#define LEAF_H
class Leaf {
    int nrOfPositives;
    int nrOfNegatives;
public:
    double probability;

    void incrementPositive();
    void incrementNegative();
};
#endif
