#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"

#include "detector/nn/Patch.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);

    string sequenceKey = args.getString("sequence");
    Sequence sequence = Sequence(sequenceKey);

    Frame* currentFrame = sequence.next();
    Box* box = sequence.initBox;

    Box* b1 = sequence.initBox;
    Box* b2 = new Box(0, box->x1 + 1.4, box->y1 + 5.9, box->x2 - 5.3, box->y2 -9.121);

    Patch* p1 = new Patch(currentFrame, b1);
    printf("%s", p1->toString().c_str());

    Patch* p2 = new Patch(currentFrame, b2);
    printf("%s", p2->toString().c_str());

}
