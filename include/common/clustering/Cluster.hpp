#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <deque>
#include <unordered_map>

#include "common/Logging.hpp"
#include "common/clustering/Distance.hpp"

using namespace std;

class Cluster {
private:
    static int clusterId;
public:
    int id;
    int size;
    vector<int> cluster;

    Cluster(int id);

    void add(int newItem);
    Cluster* merge(Cluster* other);

    static int newClusterId();
    static vector<Cluster*> build(vector<Distance*> distances, int nrOfDistances, double maxDistance);
};
#endif
