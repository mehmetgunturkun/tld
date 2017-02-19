#include "common/Arguments.hpp"
#include "common/Logging.hpp"

#include "core/BoxIterator.hpp"

#include "testbase/Sequence.hpp"

int main(int argc, char** argv) {

    Arguments* args = new Arguments(argc, argv);
    string sequenceKey = args->getString("sequence");

    Sequence* sequence = new Sequence(sequenceKey);
    Frame* frame = sequence->next();
    Box* box = sequence->initBox;

    for (int i = 0; i < 10; i++) {
        BoxIterator* iterator = new BoxIterator(frame, box);
        while (iterator->hasNext()) {
            Box* sampleBox = iterator->next();
            println("%4.3f, %4.3f, %4.3f, %4.3f", sampleBox->x1,sampleBox->y1,sampleBox->x2,sampleBox->y2);
        }
    }
}
