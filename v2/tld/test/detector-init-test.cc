#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"

#include "detector/Detector.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);

    string sequenceKey = args.getString("sequence");
    Sequence sequence = Sequence(sequenceKey);

    Frame* firstFrame = sequence.next();
    Box* firstBox = sequence.initBox;
    vector<Box*> boxList = { firstBox };

    Detector* detector = new Detector();
    detector->init(firstFrame, boxList);

    detector->eClassifier->dumpEnsembleClassifier();

}
