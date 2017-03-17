#include "common/Arguments.hpp"
#include "core/Frame.hpp"

int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);

    string fileName = args->getString("fileName");

    for (int i = 0; i < 20000000; i++) {
        Option<Frame*> maybeFrame = Frame::fromFile(i, fileName);
        if (maybeFrame.isDefined()) {
            Frame* frame = maybeFrame.get();
            println("%s", frame->name.c_str());

            delete frame;
        } else {
            println("%s", ("There is no valid frame under: " + fileName).c_str());
        }
    }
}
