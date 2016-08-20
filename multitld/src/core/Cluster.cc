#include "core/Cluster.hpp"

int Cluster::gId = 0;

Cluster::Cluster(int gId) {
    groupId = gId;
    size = 0;
}

void Cluster::add(int point) {
    pointList.push_back(point);
    size += 1;
}

Cluster* Cluster::merge(Cluster* other) {
    Cluster* c = new Cluster(gId);

    for (int i = 0; i < this->size; i++) {
        c->add(pointList[i]);
    }

    for (int i = 0; i < other->size; i++) {
        c->add(other->pointList[i]);
    }

    return c;
}
