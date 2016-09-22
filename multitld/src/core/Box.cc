#include "core/Box.hpp"

Option<Box>* Box::None = new Option<Box>();

Box::Box(int id, float x1, float y1, float x2, float y2) {
    this->id = id;

    this->x1 = x1;
    this->y1 = y1;

    this->x2 = x2;
    this->y2 = y2;
    this->width = x2 - x1 + 1;
    this->height = y2 - y1 + 1;
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

Box* Box::sum(Box* other) {
    double x1 = (this->x1 + other->x1);
    double y1 = (this->y1 + other->y1);
    double x2 = (this->x2 + other->x2);
    double y2 = (this->y2 + other->y2);

    Box* sum = new Box(0, x1, y1, x2, y2);
    return sum;
}

Box* Box::divide(int n) {
    this->x1 = this->x1 / n;
    this->x2 = this->x2 / n;
    this->y1 = this->y1 / n;
    this->y2 = this->y2 / n;
    return this;
}
