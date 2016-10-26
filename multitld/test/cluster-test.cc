#include "core/ScoredBox.hpp"

int main(int argc, char** args) {
    vector<ScoredBox*> scoredBoxList;
    scoredBoxList.push_back(new ScoredBox(new Box(13213, 129, 125, 219, 164)));
    scoredBoxList.push_back(new ScoredBox(new Box(13214, 133, 125, 223, 164)));
    scoredBoxList.push_back(new ScoredBox(new Box(13215, 137, 125, 227, 164)));
    scoredBoxList.push_back(new ScoredBox(new Box(13271, 133, 129, 223, 168)));

    vector<ScoredBox*> clusteredBoxList = ScoredBox::cluster(scoredBoxList, 4);

    printf("Clustered Boxes: %d\n", (int) clusteredBoxList.size());
    for (int i = 0; i < (int) clusteredBoxList.size(); i++) {
        ScoredBox* scoredBox = clusteredBoxList[i];
        printf("%s\n", scoredBox->box->toCharArr());
    }
    printf("==================================\n");

}
