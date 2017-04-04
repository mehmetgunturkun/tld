#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"

#include "tracker/Tracker.hpp"

int run(Arguments args);

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);
    for (int i = 0; i < 1; i++) {
        run(args);
    }
}

int run(Arguments args) {
    string sequenceKey = args.getString("sequence");
    Sequence sequence = Sequence(sequenceKey);

    Frame* prevFrame = sequence.next();
    Box* initBox = sequence.initBox;
    vector<Box*> boxList = { initBox };

    Tracker* tracker = new Tracker();

    int no = 1;
    while (sequence.hasNext() && no < 10) {
        Frame* currentFrame = sequence.next();
        // println("%s is going to be processed", currentFrame->name.c_str());

        boxList = tracker->track(prevFrame, currentFrame, boxList);
        if (boxList[0] == nullptr) {
          println(RED("Tracker stopped processing"));
          return 1;
        }
        //println("%s", boxList[0]->toCharArr());
        delete prevFrame;
        prevFrame = currentFrame;
        no += 1;
    }

    delete prevFrame;
    delete tracker;
    return 0;
}
