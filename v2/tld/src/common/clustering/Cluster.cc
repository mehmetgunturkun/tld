#include "common/clustering/Cluster.hpp"

int Cluster::clusterId = 0;

Cluster::Cluster(int id) {
    this->id = id;
    this->size = 0;
}

void Cluster::add(int newItem) {
    this->cluster.push_back(newItem);
    this->size += 1;
}

Cluster* Cluster::merge(Cluster* other) {
    int newClusterId = Cluster::newClusterId();
    Cluster* newCluster = new Cluster(newClusterId);

    for (int i = 0; i < this->size; i++) {
        newCluster->add(this->cluster[i]);
    }

    for (int i = 0; i < other->size; i++) {
        newCluster->add(other->cluster[i]);
    }

    return newCluster;
}

int Cluster::newClusterId() {
    Cluster::clusterId += 1;
    return Cluster::clusterId;
}

vector<Cluster*> Cluster::build(vector<Distance*> distances, int nrOfDistances, double maxDistance) {
    unordered_map<int, Cluster*> index;
    unordered_map<int, Cluster*> clusterIndex;

    int processedDistance = 0;
    for (int i = 0; i < nrOfDistances; i++) {
        Distance* distance = distances[i];
        processedDistance = i;

        bool p0Clustered = index.count(distance->item0) > 0;
        bool p1Clustered = index.count(distance->item1) > 0;

        if (distance->value >= maxDistance) {
            if (p0Clustered && p1Clustered) {
                //do nothing
            } else if (p0Clustered) {
                int groupId = Cluster::newClusterId();
                Cluster* c1 = new Cluster(groupId);
                c1->add(distance->item1);

                clusterIndex[c1->id] = c1;
                index[distance->item1] = c1;
            } else if (p1Clustered) {
                int groupId = Cluster::newClusterId();
                Cluster* c0 = new Cluster(groupId);
                c0->add(distance->item0);

                clusterIndex[c0->id] = c0;
                index[distance->item0] = c0;
            } else {
                int groupId0 = Cluster::newClusterId();
                Cluster* c0 = new Cluster(groupId0);
                c0->add(distance->item0);
                clusterIndex[c0->id] = c0;
                index[distance->item0] = c0;

                int groupId1 = Cluster::newClusterId();
                Cluster* c1 = new Cluster(groupId1);
                c1->add(distance->item1);
                clusterIndex[c1->id] = c1;
                index[distance->item1] = c1;
            }
        } else {
            if (p0Clustered && p1Clustered) {
                // Both have clusters.
                // Might merge those clusters,
                // if they are in different clusters
                Cluster* c0 = index[distance->item0];
                Cluster* c1 = index[distance->item1];
                if (c0->id != c1->id) {
                    Cluster* c3 = c0->merge(c1);

                    clusterIndex.erase(c0->id);
                    clusterIndex.erase(c1->id);
                    clusterIndex[c3->id] = c3;

                    for (int i = 0; i < c3->size; i++) {
                        int pid = c3->cluster[i];
                        index[pid] = c3;
                    }

                    //TODO
                    //delete c0
                    //delete c1
                }
            } else if (p0Clustered) {
                Cluster* c0 = index[distance->item0];
                c0->add(distance->item1);
                index[distance->item1] = c0;
            } else if (p1Clustered) {
                Cluster* c1 = index[distance->item1];
                c1->add(distance->item0);
                index[distance->item0] = c1;
            } else {
                int groupId = Cluster::newClusterId();
                Cluster* c0 = new Cluster(groupId);

                c0->add(distance->item0);
                c0->add(distance->item1);

                clusterIndex[c0->id] = c0;
                index[distance->item0] = c0;
                index[distance->item1] = c0;
            }
        }
    }

    if (processedDistance + 1 < nrOfDistances) {
        println(YELLOW("What the fuck %d < %d"), processedDistance,  nrOfDistances);
    }

    vector<Cluster*> clusters;
    for ( auto it = clusterIndex.begin(); it != clusterIndex.end(); ++it ) {
        Cluster* c = it->second;
        clusters.push_back(c);
    }
    return clusters;
}
