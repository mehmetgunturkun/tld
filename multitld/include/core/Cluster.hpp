#ifndef CLUSTER_H
#define CLUSTER_H

#include "common/Colors.hpp"
#include <vector>
#include <deque>
#include <unordered_map>

using namespace std;

class Distance {
public:
    int point0;
    int point1;
    double value;
    Distance(int p0, int p1, double v) {
        point0 = p0;
        point1 = p1;
        value = v;
    }
};

class Cluster {
private:
    static int gId;
public:
    int groupId;
    int size;
    vector<int> pointList;

    Cluster(int gId);

    void add(int point);
    Cluster* merge(Cluster* other);

    static int getId() {
        gId += 1;
        return gId;
    }

    static vector<Cluster*> build(vector<Distance*> distances, int nrOfDistances, double maximumDistance) {
        unordered_map<int, Cluster*> index;
        unordered_map<int, Cluster*> clusterIndex;

        int processedDistance = 0;

        for (int i = 0; i < nrOfDistances; i++) {
            Distance* distance = distances[i];
            processedDistance = i;

            bool p0Clustered = index.count(distance->point0) > 0;
            bool p1Clustered = index.count(distance->point1) > 0;

            if (distance->value >= maximumDistance) {
                if (p0Clustered && p1Clustered) {

                } else if (p0Clustered) {
                    int groupId = Cluster::getId();
                    Cluster* c1 = new Cluster(groupId);
                    c1->add(distance->point1);

                    clusterIndex[c1->groupId] = c1;
                    index[distance->point1] = c1;
                } else if (p1Clustered) {
                    int groupId = Cluster::getId();
                    Cluster* c0 = new Cluster(groupId);
                    c0->add(distance->point0);

                    clusterIndex[c0->groupId] = c0;
                    index[distance->point0] = c0;
                } else {
                    int groupId0 = Cluster::getId();
                    Cluster* c0 = new Cluster(groupId0);
                    c0->add(distance->point0);

                    clusterIndex[c0->groupId] = c0;
                    index[distance->point0] = c0;

                    int groupId1 = Cluster::getId();
                    Cluster* c1 = new Cluster(groupId1);
                    c1->add(distance->point1);

                    clusterIndex[c1->groupId] = c1;
                    index[distance->point1] = c1;
                }
            } else {
                if (p0Clustered && p1Clustered) {
                    // Both have clusters.
                    // Might merge those clusters,
                    // if they are in different clusters

                    Cluster* c0 = index[distance->point0];
                    Cluster* c1 = index[distance->point1];

                    if (c0->groupId != c1->groupId) {
                        Cluster* c3 = c0->merge(c1);

                        clusterIndex.erase(c0->groupId);
                        clusterIndex.erase(c1->groupId);
                        clusterIndex[c3->groupId] = c3;

                        for (int i = 0; i < c3->size; i++) {
                            int pid = c3->pointList[i];
                            index[pid] = c3;
                        }
                    }
                } else if (p0Clustered) {
                    Cluster* c0 = index[distance->point0];
                    c0->add(distance->point1);
                    index[distance->point1] = c0;
                } else if (p1Clustered) {
                    Cluster* c1 = index[distance->point1];
                    c1->add(distance->point0);
                    index[distance->point0] = c1;
                } else {
                    int groupId = Cluster::getId();
                    Cluster* c0 = new Cluster(groupId);

                    c0->add(distance->point0);
                    c0->add(distance->point1);

                    clusterIndex[c0->groupId] = c0;
                    index[distance->point0] = c0;
                    index[distance->point1] = c0;
                }
            }
        }

        if (processedDistance + 1 < nrOfDistances) {
            printf(COLOR_YELLOW "What the fuck %d < %d" COLOR_RESET " \n",processedDistance,  nrOfDistances);
        }

        vector<Cluster*> clusters;
        for ( auto it = clusterIndex.begin(); it != clusterIndex.end(); ++it ) {
            Cluster* c = it->second;
            clusters.push_back(c);
        }

        return clusters;
    }
};
#endif
