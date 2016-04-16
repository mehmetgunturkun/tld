#ifndef LEAF_H
#define LEAF_H
#include <string>
#include <sstream>

using namespace std;

class Leaf {
public:
    int nrOfPositives;
    int nrOfNegatives;
    double probability;

    Leaf();

    void incrementPositive();
    void incrementNegative();
    string toString() {
        stringstream ss;
        ss << "Leaf("
           << "#p = " << nrOfPositives << ", "
           << "#n = " << nrOfNegatives << ")";
        return ss.str();
    }
};
#endif
