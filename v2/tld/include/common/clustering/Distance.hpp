#ifndef DISTANCE_H
#define DISTANCE_H

class Distance {
public:
    int item0;
    int item1;
    double value;

    Distance(int i0, int i1, double value);
};

struct AscendingDistanceSortCriteria {
    bool operator()(const Distance* lhs, const Distance* rhs) const {
        return lhs->value > rhs->value;
    }
};
#endif
