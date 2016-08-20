#ifndef SCORED_BOX_H
#define SCORED_BOX_H

#include <string>
#include <unordered_map>
#include <queue>

#include "common/BoundedPriorityQueue.hpp"
#include "core/Box.hpp"
#include "core/Cluster.hpp"
#include "core/Score.hpp"

struct DistanceSortCriteria
{
    bool operator()(const Distance* lhs, const Distance* rhs) const {
        return lhs->value > rhs->value;
    }
};

class ScoredBox {
private:
    static vector<Distance*> computeDistances(vector<ScoredBox*> boxList, int nrOfBoxes) {
        BoundedPriorityQueue<Distance, DistanceSortCriteria> distanceList =
            BoundedPriorityQueue<Distance, DistanceSortCriteria>(INT_MAX);

        for (int i = 0; i < nrOfBoxes; i++) {
            ScoredBox* box1 = boxList[i];
            for (int j = i + 1; j < nrOfBoxes; j++) {
                ScoredBox* box2 = boxList[j];
                double distanceValue = 1.0 - Box::computeOverlap(box1->box, box2->box);
                Distance* distance = new Distance(i, j, distanceValue);
                distanceList += distance;
            }
        }

        vector<Distance*> distances = distanceList.toVector();
        return distances;
    }

    static vector<ScoredBox*> combineClusters(
        vector<Cluster*> cluster,
        vector<ScoredBox*> boxList) {
            int nrOfClusters = (int) cluster.size();
            vector<ScoredBox*> clusteredBoxes;
            printf("#Clusters: %d\n", nrOfClusters);

            for (int i = 0; i < nrOfClusters; i++) {
                Cluster* c = cluster[i];
                vector<int> idList = c->pointList;
                ScoredBox* box = boxList[idList[0]]->clone();
                int nrOfBoxes = c->size;
                printf("#Boxes: %d\n", nrOfBoxes);
                for (int j = 1; j < nrOfBoxes; j++) {
                    printf("%d - begin\n", j);
                    ScoredBox* otherBox = boxList[idList[j]];
                    box->merge(otherBox);
                    printf("%d - end\n", j);
                }
                printf("#Boxes: %d - end\n", nrOfBoxes);
                clusteredBoxes.push_back(box);
            }
            printf("#Clusters: %d - end\n", nrOfClusters);
            return clusteredBoxes;
    }
public:
    Box* box;
    bool isDetected;
    unordered_map<string, Score*> detailMap;

    ScoredBox(Box* box);

    ScoredBox* clone();
    void merge(ScoredBox* other);
    void withScore(string classifierKey, Score* score);
    Score* getScore(string classifierKey);
    double getScoreValue(string classifierKey);
    vector<int> getCandidateModels();

    static vector<ScoredBox*> cluster(vector<ScoredBox*> boxList, int nrOfBoxes) {
        printf("Computing distances...\n");
        vector<Distance*> distances = computeDistances(boxList, nrOfBoxes);
        printf("Building clusters...\n");
        vector<Cluster*> clusters = Cluster::build(distances, distances.size(), 0.5);
        printf("Combining clusters...\n");
        vector<ScoredBox*> clusteredBoxes = combineClusters(clusters, boxList);
        return clusteredBoxes;
    }
};
#endif
