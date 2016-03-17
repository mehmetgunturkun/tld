#include "core/Box.hpp"

Box::Box(int _id, double x_1, double y_1, double x_2, double y_2) {
    id = _id;
    x1 = x_1;
    y1 = y_1;
    x2 = x_2;
    y2 = y_2;

    width = x2 - x1;
    height = y2 - y1;
}

Box* Box::move(float dx, float dy) {
    Box* box = new Box(id, x1 + dx, y1 + dy, x2 + dx, y2 + dy);
    return box;
}

Box* Box::clone() {
    //TODO Implementation
    return this;
}
