#include "common/Arguments.hpp"
#include "core/Box.hpp"

int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);

    string boxString = args->getString("boxString");

    for (int i = 0; i < 20000000; i++) {
        Option<Box*> maybeBox = Box::fromLine(boxString);
        if (maybeBox.isDefined()) {
            Box* box = maybeBox.get();
            println("%s", box->toCharArr());

            // delete box;
        } else {
            println("%s", ("There is no valid box in string: " + boxString).c_str());
        }
    }
}
