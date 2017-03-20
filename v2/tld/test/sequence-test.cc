#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"

int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);

    string sequenceKey = args->getString("sequence");

    Sequence sequence = Sequence(sequenceKey);
    println("mc1000");
    while (sequence.hasNext()) {
      println("mc2000");
       Frame* currentFrame = sequence.next();
       println("%s is going to be processed", currentFrame->name.c_str());
    }
}
