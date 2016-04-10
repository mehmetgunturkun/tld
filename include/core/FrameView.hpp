#ifndef FRAME_VIEW_HPP
#define FRAME_VIEW_HPP

#include "core/Frame.hpp"
#include "core/Box.hpp"

class FrameView {
public:
    Mat* underlying;

    FrameView(Frame* frame);
    FrameView* addBox(Box* frame, Scalar* color = FrameView::RED);
    
    static Scalar* RED;
    static Scalar* BLUE;
    static Scalar* GREEN;
    static Scalar* ORANGE;
    static Scalar* PURPLE;
    static Scalar* YELLOW;
};
#endif
