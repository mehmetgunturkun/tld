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
        std::reverse(distances.begin(), distances.end());
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

                int nrOfBoxes = c->size;
                vector<ScoredBox*> scoredBoxListPerCluster(nrOfBoxes);
                for (int j = 0; j < nrOfBoxes; j++) {
                    scoredBoxListPerCluster[j] = boxList[idList[j]];
                }

                ScoredBox* box = ScoredBox::merge(scoredBoxListPerCluster, nrOfBoxes);
                clusteredBoxes.push_back(box);
            }
            return clusteredBoxes;
    }
public:
    Box* box;
    bool isDetected;
    unordered_map<string, Score*> detailMap;

    ScoredBox();
    ScoredBox(Box* box);

    ScoredBox* clone();
    ScoredBox* sum(ScoredBox* other);
    ScoredBox* divide(int n);

    void withScore(string classifierKey, Score* score);
    Score* getScore(string classifierKey);
    double getScoreValue(string classifierKey, int modelId);

    vector<int> getCandidateModels();
    vector<int> getCandidateModels(string classifierKey);
    bool isClassified(string classifierKey, int modelId);
    bool isScored(string classifierKey);

    static vector<ScoredBox*> cluster(vector<ScoredBox*> boxList, int nrOfBoxes) {
        if (nrOfBoxes == 1) {
            return boxList;
        }
        vector<Distance*> distances = computeDistances(boxList, nrOfBoxes);
        vector<Cluster*> clusters = Cluster::build(distances, (int) distances.size(), 0.5);
        vector<ScoredBox*> clusteredBoxes = combineClusters(clusters, boxList);
        return clusteredBoxes;
    }

    static ScoredBox* merge(vector<ScoredBox*> scoredBoxList, int nrOfBoxes) {
        ScoredBox* tempBox = scoredBoxList[0]->clone();
        for (int i = 1; i < nrOfBoxes; i++) {
            ScoredBox* b = scoredBoxList[i];
            tempBox = tempBox->sum(b);
        }
        tempBox = tempBox->divide(nrOfBoxes);
        return tempBox;
    }
};

struct ScoredBoxOverlapOrdered {
    bool operator() (ScoredBox* box1, ScoredBox* box2) {
        return box1->box->overlap >= box2->box->overlap;
    }

    bool compare(ScoredBox* box1, ScoredBox* box2) {
        return box1->box->overlap > box2->box->overlap;
    }
};
#endif
