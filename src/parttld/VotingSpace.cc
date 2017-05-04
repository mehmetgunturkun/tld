#include "parttld/VotingSpace.hpp"

inline double euclidian(Point2f* p0, Point2f* p1);
int kd_insert2(struct kdtree *tree, double x, double y);
struct kdres *kd_nearest_range2(struct kdtree *tree, double x, double y, double range);
struct kdres *kd_nearest2(struct kdtree *tree, double x, double y);
struct kdres* kd_nearest_n2(struct kdtree* tree, double x, double y, int num);

VotingSpace::VotingSpace() {
    kd = kd_create(2);
}

VotingSpace::~VotingSpace() {
    kd_free(kd);
}

void VotingSpace::vote(double x, double y, double vote) {
    kd_insert2(kd, x, y);
}

Point2f* VotingSpace::result() {
    double pos[2];
    struct kdres* random = kd_nearest_n2(kd, 0, 0, 1);
    kd_res_item(random, pos);
    Point2f* randomPoint = new Point2f(pos[0], pos[1]);

    Point2f* center = computeTheCenter(point);
    return center;
}

Point2f* VotingSpace::computeTheCenter(Point2f* currentPoint) {
    Point2f* nextCenter = computeNextCenter(currentPoint);
    double distance = euclidian(nextCenter, currentPoint);
    if (distance < MEAN_SHIFT_THRESHOLD) {
        return nextCenter;
    } else {
        return computeTheCenter(nextCenter);
    }
}

Point2f* VotingSpace::computeNextCenter(Point2f* point) {
    struct kdres* neighbours = kd_nearest_range2(kd, point->x, point->y, 100);
    double pos[2];

    double meanX = 0.0;
    double meanY = 0.0;
    int nrOfNeighbours = kd_res_size(neighbours);

    while( !kd_res_end( neighbours ) ) {
        kd_res_item(neighbours, pos);
        meanX += pos[0];
        meanY += pos[1];
        kd_res_next( neighbours );
    }

    Point2f* newPoint = new Point2f(meanX / nrOfNeighbours, meanY / nrOfNeighbours);

    kd_res_free(neighbours);
    return newPoint;
}

inline double euclidian(Point2f* p0, Point2f* p1) {
    double dx = p0->x - p1->x;
    double dy = p0->y - p1->y;
    return sqrt(dx * dx + dy* dy);
}

int kd_insert2(struct kdtree *tree, double x, double y) {
	double buf[2];
	buf[0] = x;
	buf[1] = y;
	return kd_insert(tree, buf, 0);
}

struct kdres *kd_nearest_range2(struct kdtree *tree, double x, double y, double range) {
	double buf[2];
	buf[0] = x;
	buf[1] = y;
	return kd_nearest_range(tree, buf, range);
}

struct kdres *kd_nearest2(struct kdtree *tree, double x, double y) {
	double pos[2];
	pos[0] = x;
	pos[1] = y;
	return kd_nearest(tree, pos);
}

struct kdres* kd_nearest_n2(struct kdtree* tree, double x, double y, int num) {
    double pos[2];
    pos[0] = x;
    pos[1] = y;
    return kd_nearest_n(tree, pos, num);
}
