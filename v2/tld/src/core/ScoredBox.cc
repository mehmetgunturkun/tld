#include "core/ScoredBox.hpp"

ScoredBox::ScoredBox(Box* box) {
    this->box = box;
    this->isDetected = false;
}

ScoredBox::~ScoredBox() {
    delete box;
    for ( auto it = scoreMap.begin(); it != scoreMap.end(); ++it ) {
        Score* score = it->second;
        delete score;
    }
}

ScoredBox* ScoredBox::withScore(string key, Score* score) {
    scoreMap[key] = score;
    return this;
}

Score* ScoredBox::getScore(string key) {
    return scoreMap[key];
}

double ScoredBox::getScoreValue(string key, int modelId) {
    Score* score = scoreMap[key];
    return score->getValue(modelId);
}

Score* ScoredBox::remove(string key) {
    Score* score = scoreMap[key];
    scoreMap.erase(key);
    return score;
}

bool ScoredBox::isClassified(string classifierKey, int modelId) {
    Score* score = getScore(classifierKey);
    if (score == NULL) {
        return false;
    } else {
        vector<int> classifiedModelIds = score->classifiedModelIds;
        return std::find(classifiedModelIds.begin(), classifiedModelIds.end(), modelId) != classifiedModelIds.end();
    }
}

bool ScoredBox::isScored(string classifierKey) {
    Score* scoreItem = getScore(classifierKey);
    return scoreItem != NULL;
}

ScoredBox* ScoredBox::clone() {
    Box* cloneBox = this->box->clone();
    ScoredBox* clone = new ScoredBox(cloneBox);
    clone->isDetected = this->isDetected;
    for ( auto it = this->scoreMap.begin(); it != this->scoreMap.end(); ++it ) {
        Score* score = it->second;
        clone->scoreMap[it->first] = score->clone();
    }
    return clone;
}

vector<Distance*> computeDistances(vector<ScoredBox*> boxList, int nrOfBoxes);
vector<ScoredBox*> combineClusters(vector<Cluster*> cluster, vector<ScoredBox*> boxList);

template <typename T>
void free_vector(vector<T*> vector) {
    int size = (int) vector.size();
    for (int i = 0; i < size; i++) {
        delete vector[i];
    }
}

vector<ScoredBox*> ScoredBox::cluster(vector<ScoredBox*> boxList, int nrOfBoxes) {
    if (nrOfBoxes == 1) {
        vector<ScoredBox*> clusteredBoxes = { boxList[0]->clone() };
        return clusteredBoxes;
    }

    vector<Distance*> distances = computeDistances(boxList, nrOfBoxes);
    vector<Cluster*> clusters = Cluster::build(distances, (int) distances.size(), 0.5);
    vector<ScoredBox*> clusteredBoxes = combineClusters(clusters, boxList);

    free_vector(distances);
    free_vector(clusters);

    return clusteredBoxes;

}

vector<Distance*> computeDistances(vector<ScoredBox*> boxList, int nrOfBoxes) {
    BoundedPriorityQueue<Distance, AscendingDistanceSortCriteria> distanceList =
        BoundedPriorityQueue<Distance, AscendingDistanceSortCriteria>(INT_MAX);

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

vector<ScoredBox*> combineClusters(
    vector<Cluster*> cluster,
    vector<ScoredBox*> boxList) {
        int nrOfClusters = (int) cluster.size();
        vector<ScoredBox*> clusteredBoxes;

        for (int i = 0; i < nrOfClusters; i++) {
            Cluster* c = cluster[i];
            vector<int> idList = c->cluster;

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

ScoredBox* ScoredBox::merge(vector<ScoredBox*> scoredBoxList, int nrOfBoxes) {
    ScoredBox* tempBox = scoredBoxList[0]->clone();
    for (int i = 1; i < nrOfBoxes; i++) {
        ScoredBox* b = scoredBoxList[i];
        tempBox = tempBox->sum(b);
    }
    tempBox = tempBox->divide(nrOfBoxes);
    return tempBox;
}

ScoredBox* ScoredBox::sum(ScoredBox* other) {
    this->box = this->box->sum(other->box);
    for ( auto it = this->scoreMap.begin(); it != this->scoreMap.end(); ++it ) {
        Score* thisScore = it->second;
        Score* otherScore = other->scoreMap[it->first];

        Score* newScore = thisScore->sum(otherScore);
        this->scoreMap[it->first] = newScore;
    }
    return this;
}

ScoredBox* ScoredBox::divide(int n) {
    this->box = this->box->divide(n);
    for ( auto it = this->scoreMap.begin(); it != this->scoreMap.end(); ++it ) {
        Score* thisScore = it->second;
        thisScore = thisScore->divide(n);
    }
    return this;
}
