#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"

#include "detector/ensemble/EnsembleClassifier.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);

    string sequenceKey = args.getString("sequence");
    Sequence sequence = Sequence(sequenceKey);

    Frame* firstFrame = sequence.next();
    Frame* secondFrame = sequence.next();
    Box* initBox = sequence.initBox;
    vector<Box*> boxList = { initBox };

    EnsembleClassifier* eClassifier = new EnsembleClassifier();

    //Initialization
    eClassifier->init(firstFrame, boxList);

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

    eClassifier->train(set1, 0, 0.0);

    for (int i = 0; i < (int) positiveBoxList.size(); i++) {
        Box* b = positiveBoxList[i];
        delete b;
    }

    for (int i = 0; i < (int) negativeBoxList.size(); i++) {
        Box* b = negativeBoxList[i];
        delete b;
    }

    //Classification
    Box* anyBox = new Box(0, 8, 10, 20, 20);
    ScoredBox* anyScoredBox = new ScoredBox(anyBox);

    eClassifier->classify(secondFrame, anyScoredBox);
    delete anyScoredBox;

    printf("Testing Update...\n");

    //Update
    ScoredBox* sb1 = new ScoredBox(new Box(0, 8, 10, 20, 20));
    ScoredBox* sb2 = new ScoredBox(new Box(0, 9, 11, 21, 21));
    ScoredBox* sb3 = new ScoredBox(new Box(0, 10, 12, 22, 22));
    ScoredBox* sb4 = new ScoredBox(new Box(0, 12, 11, 21, 21));
    ScoredBox* sb5 = new ScoredBox(new Box(0, 13, 10, 20, 20));
    vector<ScoredBox*> positiveBoxListUpdate = { sb1, sb2, sb3, sb4, sb5 };

    ScoredBox* sb6 = new ScoredBox(new Box(0, 108, 100, 200, 200));
    eClassifier->score(secondFrame, sb6);

    ScoredBox* sb7 = new ScoredBox(new Box(0, 109, 101, 201, 201));
    eClassifier->score(secondFrame, sb7);

    ScoredBox* sb8 = new ScoredBox(new Box(0, 110, 102, 202, 202));
    eClassifier->score(secondFrame, sb8);

    ScoredBox* sb9 = new ScoredBox(new Box(0, 112, 101, 201, 201));
    eClassifier->score(secondFrame, sb9);

    ScoredBox* sb10 = new ScoredBox(new Box(0, 113, 100, 200, 200));
    eClassifier->score(secondFrame, sb10);

    vector<ScoredBox*> negativeBoxListUpdate = { sb6, sb7, sb8, sb9, sb10 };

    TrainingSet<ScoredBox> set2 = TrainingSet<ScoredBox>(
        secondFrame,
        positiveBoxListUpdate,
        negativeBoxListUpdate,
        false
    );

    eClassifier->train(set2, 0, 0.0);

    for (int i = 0; i < (int) positiveBoxListUpdate.size(); i++) {
        ScoredBox* b = positiveBoxListUpdate[i];
        delete b;
    }

    for (int i = 0; i < (int) negativeBoxListUpdate.size(); i++) {
        ScoredBox* b = negativeBoxListUpdate[i];
        delete b;
    }

    delete eClassifier;
    delete firstFrame;
    delete secondFrame;

    return 0;

}
