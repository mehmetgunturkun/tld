#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"
#include "core/util/ImageBuilder.hpp"

#include "detector/Detector.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);

    string sequenceKey = args.getString("sequence");
    Sequence sequence = Sequence(sequenceKey);

    Frame* firstFrame = sequence.next();
    Frame* secondFrame = sequence.next();
    Box* firstBox = sequence.initBox;
    vector<Box*> boxList = { firstBox };

    Detector* detector = new Detector();
    detector->init(firstFrame, boxList);

    vector<ScoredBox*> scoredBoxList = detector->detect(secondFrame);
    int nrOfScoredBox = (int) scoredBoxList.size();

    for (int i = 0; i < nrOfScoredBox; i++) {
        ScoredBox* scoredBox = scoredBoxList[i];
        delete scoredBox;
    //     if (scoredBox->isDetected) {
    //         ImageBuilder builder = ImageBuilder(secondFrame);
    //         builder.withBox(scoredBox->box).display(0);
    //     }
    }


    delete firstFrame;
    delete secondFrame;
    delete detector;
}
