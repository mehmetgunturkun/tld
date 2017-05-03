#include <iostream>
#include <fstream>

#include "common/Arguments.hpp"
#include "core/Sequence.hpp"
#include "parttld/PartTLD.hpp"

int main(int argc, char** args) {
    Arguments arguments = Arguments(argc, args);

    string key = arguments.getString("sequence");
    int limit = arguments.getIntOrDefault("limit", NO_LIMIT);

    printf("Going to run for %s\n", key.c_str());
    Sequence sequence = Sequence(key, 0, limit);
    Frame* firstFrame = sequence.next();
    Box* initBox = sequence.initBox;
    initBox->isValid = true;

    initBox->x1 -= 1.0;
    initBox->y1 -= 1.0;
    initBox->x2 -= 1.0;
    initBox->y2 -= 1.0;

    PartTLD* tld = new PartTLD();

    Shape* shape0 = tld->init(firstFrame, initBox);

    Frame* previous = firstFrame;
    while (sequence.hasNext()) {
        Frame* current = sequence.next();
        printf("Frame(%d) >> Frame(%d)\n", previous->id, current->id);

        Shape* shape1 = tld->track(previous, current, shape0);

        vector<Box*> currentCorrectedBoxList = shape1->parts;
        ImageBuilder* builder = new ImageBuilder(current);
        int nrOfModels = (int) currentCorrectedBoxList.size();
        for (int i = 0; i < nrOfModels; i++) {
            Box* box = currentCorrectedBoxList[i];

            if (box != nullptr) {
                // string boxString = box->toTLDString();
                builder->withBox(box, Colors::colors[i]);
            } else {
                printf("Frame(%d) >> Frame(%d) FAILED\n", previous->id, current->id);
            }
        }

        builder->withCircle(shape1->center, 10);

        builder->display(1);

        shape0 = shape1;

        delete previous;
        previous = current;
    }

    delete previous;
    delete tld;

    return EXIT_SUCCESS;
}
