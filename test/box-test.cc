#include "common/Arguments.hpp"
#include "core/Box.hpp"
#include "detector/training/TrainingSet.hpp"
int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);

    string boxString = args->getString("boxString");

    for (int i = 0; i < 1000; i++) {
        Option<Box*> maybeBox = Box::fromLine(boxString);
        if (maybeBox.isDefined()) {
            Box* box = maybeBox.get();
            println("%d. %s", i, box->toCharArr());

            delete box;
        } else {
            println("%s", ("There is no valid box in string: " + boxString).c_str());
        }
    }
}
