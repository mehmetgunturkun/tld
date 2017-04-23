#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"

#include "detector/nn/NearestNeighborClassifier.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);

    string sequenceKey = args.getString("sequence");
    Sequence sequence = Sequence(sequenceKey);

    Frame* firstFrame = sequence.next();
    Box* initBox = sequence.initBox;
    vector<Box*> boxList = { initBox };

    NearestNeighborClassifier* nnClassifier = new NearestNeighborClassifier();
    nnClassifier->init(firstFrame, boxList);

    Box* b1 = new Box(0, 8, 10, 20, 20);
    Box* b2 = new Box(0, 9, 11, 21, 21);
    Box* b3 = new Box(0, 10, 12, 22, 22);
    Box* b4 = new Box(0, 12, 11, 21, 21);
    Box* b5 = new Box(0, 13, 10, 20, 20);
    vector<Box*> positiveBoxList = { b1, b2, b3, b4, b5};

    Box* b6 = new Box(0, 108, 100, 200, 200);
    Box* b7 = new Box(0, 109, 101, 201, 201);
    Box* b8 = new Box(0, 110, 102, 202, 202);
    Box* b9 = new Box(0, 112, 101, 201, 201);
    Box* b10 = new Box(0, 113, 100, 200, 200);
    vector<Box*> negativeBoxList = { b6, b7, b8, b9, b10 };

    TrainingSet<Box> set1 = TrainingSet<Box>(
        firstFrame,
        positiveBoxList,
        negativeBoxList,
        false
    );

    nnClassifier->train(set1, 0);

    for (int i = 0; i < (int) positiveBoxList.size(); i++) {
        Box* b = positiveBoxList[i];
        delete b;
    }

    for (int i = 0; i < (int) negativeBoxList.size(); i++) {
        Box* b = negativeBoxList[i];
        delete b;
    }

    delete nnClassifier;
    delete firstFrame;

    return 0;

}
