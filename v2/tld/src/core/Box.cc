#include "core/Box.hpp"

Box::Box(double x1, double y1, double x2, double y2) {
    this->id = 0;

    this->x1 = x1;
    this->y1 = y1;

    this->x2 = x2;
    this->y2 = y2;
    this->width = x2 - x1 + 1;
    this->height = y2 - y1 + 1;

    this->mean = 0.0;
    this->variance = 0.0;
    this->overlap = 0.0;

    this->nrOfPoints = 0.0;
    this->isValid = false;
}

Box::Box(int id, double x1, double y1, double x2, double y2) {
    this->id = id;

    this->x1 = x1;
    this->y1 = y1;

    this->x2 = x2;
    this->y2 = y2;
    this->width = x2 - x1 + 1;
    this->height = y2 - y1 + 1;

    this->mean = 0.0;
    this->variance = 0.0;
    this->overlap = 0.0;

    this->nrOfPoints = 0.0;
    this->isValid = false;
}

Box* Box::move(double dx, double sx, double dy, double sy) {
    Box* box = new Box(id, x1 - sx + dx, y1 - sy + dy, x2 + sx + dx, y2 + sy + dy);
    return box;
}

Box* Box::clone() {
    Box* clone = new Box(id, x1, y1, x2, y2);
    return clone;
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

void Box::print() {
    printf("Box(%a, %a, %a, %a)\n", x1, y1, x2, y2);
}

string Box::toString() {
    stringstream ss;
    ss  << "Box("
        << id << ", "
        << x1 << ", "
        << y1 << ", "
        << x2 << ", "
        << y2;
    return ss.str();
}

string Box::toTLDString() {
    stringstream ss;
    ss  << (x1 + 1.0) << ","
        << (y1 + 1.0) << ","
        << (x2 + 1.0) << ","
        << (y2 + 1.0);
    return ss.str();
}

const char* Box::toCharArr() {
    string str = this->toString();
    return str.c_str();
}

Option<Box*> Box::fromLine(string line) {
    StringStream stream = StringStream(line, ',');

    double x1 = stod(stream.next());
    double y1 = stod(stream.next());
    double x2 = stod(stream.next());
    double y2 = stod(stream.next());

    if (isnan(x1) || isnan(x2) || isnan(y1) || isnan(y2)) {
        Option<Box*> none = Option<Box*>();
        return none;
    }

    Box* box = new Box(x1, y1, x2, y2);
    Option<Box*> maybeBox = Option<Box*>(box);
    return maybeBox;
}

double Box::computeOverlap(Box* b1, Box* b2) {
    double x11 = b1->x1;
    double x12 = b1->x2;
    double y11 = b1->y1;
    double y12 = b1->y2;

    double x21 = b2->x1;
    double x22 = b2->x2;
    double y21 = b2->y1;
    double y22 = b2->y2;

    if (x11 > x22 ||
        y11 > y22 ||
        x12 < x21 ||
        y12 < y21) {
            return 0;
    }

    double intersectionX = MIN_DOUBLE(x12, x22) - MAX_DOUBLE(x11, x21) + 1;
    double intersectionY = MIN_DOUBLE(y12, y22) - MAX_DOUBLE(y11, y21) + 1;
    double intersection = intersectionX * intersectionY;

    double area1 = (x12 - x11 + 1) * (y12 - y11 + 1);
    double area2 = (x22 - x21 + 1) * (y22 - y21 + 1);
    double overlap = intersection / (area1 + area2 - intersection);

    if (overlap > 1.0) {
        printf("Compare:\n");
        printf("\t%s:\n", b1->toCharArr());
        printf("\t%s:\n", b2->toCharArr());

        printf("MIN(%g, %g) = %g\n", x12, x22, MIN_DOUBLE(x12, x22));
        printf("MAX(%g, %g) = %g\n", x11, x21, MAX_DOUBLE(x11, x21));
        printf("MIN(%g, %g) = %g\n", y12, y22, MIN_DOUBLE(y12, y22));
        printf("MAX(%g, %g) = %g\n", y11, y21, MAX_DOUBLE(y11, y21));

        printf("INTx: %g\n", intersectionX);
        printf("INTy: %g\n", intersectionY);
        printf("INTxy: %g\n", intersection);
        printf("AREA1: %g\n", area1);
        printf("AREA2: %g\n", area2);
    }

    return overlap;
}
