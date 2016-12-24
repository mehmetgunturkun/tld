#include "core/Image.hpp"
#include "testbase/Sequence.hpp"
#include "util/ImageBuilder.hpp"

int main(int argc, char** args) {
    string key(args[2]);

    printf("Going to run for %s\n", key.c_str());
    Sequence* sequence = new Sequence(key);
    while (sequence->hasNext()) {
        Frame* frame = sequence->next();
        ImageBuilder* builder = new ImageBuilder(frame);
        builder->display(1);
    }
    return EXIT_SUCCESS;
}
