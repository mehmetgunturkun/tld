#include "core/Box.hpp"

Option<Box>* Box::None = new Option<Box>();

Box::Box(int id, float x1, float y1, float x2, float y2) {
    this->id = id;

    this->x1 = x1;
    this->y1 = y1;

    this->x2 = x2;
    this->y2 = y2;
    this->width = x2 - x1;
    this->height = y2 - y1;
}

Box* Box::move(float dx, float dy) {
    Box* box = new Box(id, x1 + dx, y1 + dy, x2 + dx, y2 + dy);
    return box;
}

Box* Box::clone() {
    Box* clone = new Box(id, x1, y1, x2, y2);
    return clone;
}

vector<Box*> Box::splitTwo() {
    Box* b1 = new Box(id + 1, this->x1, this->y1, (this->x1 + this->x2) / 2, this->y2);
    Box* b2 = new Box(id + 1, (this->x1 + this->x2) / 2, this->y1, this->x2, this->y2);
    vector<Box*> boxList = {b1, b2};
    return boxList;
}
