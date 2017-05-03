#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"

#include "core/Box.hpp"
#include "core/ScoredBox.hpp"

#include "detector/ensemble/EnsembleClassifier.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);

    string sequenceKey = args.getString("sequence");
    Sequence sequence = Sequence(sequenceKey);

    Frame* currentFrame = sequence.next();
    Box* box = sequence.initBox;
    vector<Box*> boxList = { box };

    EnsembleClassifier* eClassifier = new EnsembleClassifier();
    eClassifier->init(currentFrame, boxList);

    eClassifier->dumpEnsembleClassifier();

    printf("Box: %s\n", box->toString().c_str());

    ScoredBox* scoredBox = new ScoredBox(box);
    eClassifier->classify(currentFrame, scoredBox);

    EnsembleScore* eScore = (EnsembleScore*) scoredBox->getScore("ensemble");
    CodeVector* codeVector = eScore->binaryCodes;
    vector<int> binaryCodes = codeVector->codes;
    int nrOfCodes = (int) binaryCodes.size();

    for (int i = 0; i< nrOfCodes; i++) {
        printf("%d, ", binaryCodes[i]);
    }
    printf("\n");

}
