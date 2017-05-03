#ifndef VOTING_SPACE_H
#define VOTING_SPACE_H

#include "opencv2/core/core.hpp"
#include "kdtree.h"

using namespace std;
using namespace cv;

#define MEAN_SHIFT_THRESHOLD 0.000001

class VotingSpace {
private:
    struct kdtree *kd;
public:
    VotingSpace();
    ~VotingSpace();

    void vote(double x, double y, double vote);
    Point2f* computeTheCenter(Point2f* currentPoint);
    Point2f* computeNextCenter(Point2f* currentPoint);

    Point2f* result();
};
#endif
