#include "core/FrameView.hpp"

Scalar* FrameView::RED = new Scalar(0, 0, 255);
Scalar* FrameView::YELLOW = new Scalar(0, 255, 255);
Scalar* FrameView::ORANGE = new Scalar(0, 128, 255);
Scalar* FrameView::GREEN = new Scalar(0, 255, 0);
Scalar* FrameView::PURPLE = new Scalar(204, 0, 102);
Scalar* FrameView::BLUE = new Scalar(255, 0, 0);

FrameView::FrameView(Frame* frame) {
    underlying = new Mat(frame->colored->clone());
}

FrameView* FrameView::addBox(Box* box, Scalar* color) {
    cv::rectangle(
        *underlying,
        Point2i(box->x1, box->y1),
        Point2i(box->x2 ,box->y2),
        *color, 1, 8, 0
    );
    return this;
}
