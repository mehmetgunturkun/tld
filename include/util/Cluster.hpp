#ifndef CLUSTER_H
#define CLUSTER_H

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

    Cluster(int groupId);

    void add(int point);
    Cluster* merge(Cluster* other);


    static int getId() {
        gId += 1;
        return gId;
    }

    static vector<Cluster*> build(deque<Distance*> distances, double maximumDistance) {
        unordered_map<int, Cluster*> index;
        unordered_map<int, Cluster*> clusterIndex;

        while (distances.size() > 0) {
            Distance* distance = distances.front();
            distances.pop_front();

            if (distance->value > maximumDistance) {
                break;
            }

            bool p0Clustered = index.count(distance->point0) > 0;
            bool p1Clustered = index.count(distance->point1) > 0;

            if (p0Clustered && p1Clustered) {
                // Both have clusters.
                // Might merge those clusters,
                // if they are in different clusters

                Cluster* c0 = index[distance->point0];
                Cluster* c1 = index[distance->point1];

                if (c0->groupId != c1->groupId) {
                    Cluster* c3 = c0->merge(c1);
                    clusterIndex[c3->groupId] = c3;
                    clusterIndex.erase(c0->groupId);
                    clusterIndex.erase(c1->groupId);

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

        vector<Cluster*> clusters;
        for ( auto it = clusterIndex.begin(); it != clusterIndex.end(); ++it ) {
            Cluster* c = it->second;
            clusters.push_back(c);
        }

        return clusters;
    }
};
#endif
