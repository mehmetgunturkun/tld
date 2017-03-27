#include "common/Arguments.hpp"
#include "core/Sequence.hpp"
#include "core/Frame.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);
    Sequence sequence = Sequence(args.getString("sequence"));

    for (int i = 0; i < 1; i++) {
        Frame* frame = sequence.next();
        println("%s", frame->name.c_str());

        IplImage* grayscale = frame->grayscale;

        long total = 0;

        for (int i = 0; i < frame->height; i++) {
            for (int j = 0; j < frame->width; j++) {
                char pixel = grayscale->imageData[i * frame->width + j];
                total += (uchar) pixel;
            }
        }

        println("Total: %10lu", total);

        delete frame;
    }
}
