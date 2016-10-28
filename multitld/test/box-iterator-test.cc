#include "testbase/Sequence.hpp"
#include "core/BoxIterator.hpp"
#include "util/ImageBuilder.hpp"

int main(int argc, char** argv) {
    string key(argv[1]);
    Sequence* sequence = new Sequence(key);
    Frame* firstFrame = sequence->next();
    Box* firstBox = sequence->initBox;

    BoxIterator* boxTracer = new BoxIterator(firstFrame, firstBox, 10, 24);
    int nrOfBoxes = 0;
    while (boxTracer->hasNext()) {
        Box* b = boxTracer->next();
        nrOfBoxes += 1;
        println("%s", b->toTLDString().c_str());
    }
}
