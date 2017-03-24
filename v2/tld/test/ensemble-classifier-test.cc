#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"

#include "detector/ensemble/EnsembleClassifier.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);

    string sequenceKey = args.getString("sequence");
    Sequence sequence = Sequence(sequenceKey);

    Frame* firstFrame = sequence.next();
    Box* initBox = sequence.initBox;
    vector<Box*> boxList = { initBox };

    EnsembleClassifier* eClassifier = new EnsembleClassifier();
    eClassifier->init(firstFrame, boxList);

    delete eClassifier;
    delete firstFrame;

    return 0;

}
