#include "core/Box.hpp"

Option<Box>* Box::None = new Option<Box>();

Box::Box(int id, double x1, double y1, double x2, double y2) {
    this->id = id;

    this->x1 = x1;
    this->y1 = y1;

    this->x2 = x2;
    this->y2 = y2;
    this->width = x2 - x1 + 1;
    this->height = y2 - y1 + 1;
    this->isValid = false;
}

Box::Box(int id, double x1, double y1, double x2, double y2, double w, double h) {
    this->id = id;

    this->x1 = x1;
    this->y1 = y1;

    this->x2 = x2;
    this->y2 = y2;
    this->width = x2 - x1 + 1;
    this->height = y2 - y1 + 1;
    this->isValid = false;
}

bool Box::isStandard() {
    double sum = 0.0;
    sum += x1;
    sum += x2;
    sum += y1;
    sum += y2;

    double roundedSum = 0.0;
    roundedSum += round(x1);
    roundedSum += round(x2);
    roundedSum += round(y1);
    roundedSum += round(y2);

    return sum == roundedSum;
}

Box* Box::move(double dx, double sx, double dy, double sy) {
    Box* box = new Box(id, x1 - sx + dx, y1 - sy + dy, x2 + sx + dx, y2 + sy + dy);
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
    double new_x1 = this->x1 / n;
    double new_x2 = this->x2 / n;
    double new_y1 = this->y1 / n;
    double new_y2 = this->y2 / n;

    Box* newBox = new Box(this->id, new_x1, new_y1, new_x2, new_y2);
    return newBox;
}
