#ifndef SCORED_BOX_HPP
#define SCORED_BOX_HPP

#include <string>
#include <unordered_map>
#include <queue>

#include "util/Cluster.hpp"
#include "common/BoundedPriorityQueue.hpp"
#include "core/Box.hpp"
#include "detector/common/ClassificationDetails.hpp"

using namespace std;

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

            for (int i = 0; i < nrOfClusters; i++) {
                Cluster* c = cluster[i];
                vector<int> idList = c->pointList;
                ScoredBox* box = boxList[idList[0]]->clone();
                int nrOfBoxes = c->size;
                for (int j = 1; j < nrOfBoxes; i++) {
                    ScoredBox* otherBox = boxList[idList[j]];
                    box->merge(otherBox);
                }
                clusteredBoxes.push_back(box);
            }
            return clusteredBoxes;
    }

public:
    Box* box;
    bool isDetected;
    unordered_map<string, ClassificationDetails*> detailMap;

    ScoredBox(Box* box);
    ScoredBox* clone();
    void merge(ScoredBox* other);

    void withScore(string scorerKey, ClassificationDetails* detail);

    double getScore(string scorerKey) {
        ClassificationDetails* details = detailMap[scorerKey];
        return details->score;
    }

    ClassificationDetails* getDetail(string scorerKey) {
        ClassificationDetails* details = detailMap[scorerKey];
        return details;
    }

    static vector<ScoredBox*> cluster(vector<ScoredBox*> boxList, int nrOfBoxes) {
        vector<Distance*> distances = computeDistances(boxList, nrOfBoxes);
        vector<Cluster*> clusters = Cluster::build(distances, distances.size(), 0.5);
        vector<ScoredBox*> clusteredBoxes = combineClusters(clusters, boxList);
        return clusteredBoxes;
    }
};

struct ScoredBoxOverlapOrdered {
  bool operator() (ScoredBox* box1, ScoredBox* box2) {
      return box1->box->overlap >= box2->box->overlap;
  }
};
#endif
