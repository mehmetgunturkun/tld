#include "testbase/Sequence.hpp"
#include "core/BoxIterator.hpp"
#include "util/ImageBuilder.hpp"

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("car");
    Frame* firstFrame = sequence->next();
    Box* firstBox = sequence->initBox;

    BoxIterator* boxIterator = new BoxIterator(firstFrame, firstBox, 10, 24);
    while (boxIterator->hasNext()) {
        Box* sampleBox = boxIterator->next();
        printf("%s\n", sampleBox->toString().c_str());
        ImageBuilder* builder = new ImageBuilder(firstFrame);
        builder->withBox(sampleBox, Colors::RED)->display(1);
    }
}
