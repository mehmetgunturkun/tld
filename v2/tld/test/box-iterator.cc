#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/Sequence.hpp"
#include "detector/core/BoxIterator.hpp"

int main(int argc, char** argv) {

    Arguments args = Arguments(argc, argv);

    string key = args.getString("sequence");
    // int size = gridSize[i];

    Sequence* sequence = new Sequence(key);
    Frame* firstFrame = sequence->next();
    Box* firstBox = sequence->initBox;

    BoxIterator* iterator = new BoxIterator(firstFrame, firstBox);
    while (iterator->hasNext()) {
        Box* b = iterator->next();
        delete b;
    }

    int nrOfBoxes = iterator->nrOfBoxes;
    println("There are %d boxes for %s", nrOfBoxes, key.c_str());

    delete firstFrame;
    delete iterator;
    delete sequence;

    // if (nrOfBoxes == size) {
    //     printf(GREEN("Seq(%s) => total number of boxes: %d = %d\n"), key.c_str(), nrOfBoxes, size);
    // } else {
    //     printf(RED("Seq(%s) => total number of boxes: %d ≠ %d\n"), key.c_str(), nrOfBoxes, size);
    // }

}
