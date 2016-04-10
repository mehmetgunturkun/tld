#include <cstdlib>
#include <cstdio>

#include "detector/common/BoxIterator.hpp"
#include "core/FrameView.hpp"
#include "util/Util.h"
#include "util/Dataset.hpp"

int main(int argc, char** argv) {
    Dataset* dataset = new Dataset("car");
    Frame* f = dataset->next();
    println(f->toString().c_str());

    Box* b = dataset->initBox;
    println(b->toString().c_str());

    BoxIterator* iterator = new BoxIterator(f, b, 10, 24);
    while (iterator->hasNext()) {
        FrameView* view = new FrameView(f);
        Box* nextBox = iterator->next();
        view->addBox(nextBox, FrameView::RED);
        Image::imshow("iterator", view->underlying, 1);
    }
    return EXIT_SUCCESS;
}
