#include "core/Box.hpp"

Box::Box(double x1, double y1, double x2, double y2) {
    this->id = 0;

    this->x1 = x1;
    this->y1 = y1;

    this->x2 = x2;
    this->y2 = y2;
    this->width = x2 - x1 + 1;
    this->height = y2 - y1 + 1;
}

string Box::toString() {
    stringstream ss;
    ss  << "Box("
        << id << ", "
        << x1 << ", "
        << y1 << ", "
        << x2 << ", "
        << y2 << ", "
        << width << ", "
        << height<< ")";
    return ss.str();
}

string Box::toTLDString() {
    stringstream ss;
    ss  << (x1 + 1.0) << ", "
        << (y1 + 1.0) << ", "
        << (x2 + 1.0) << ", "
        << (y2 + 1.0) << ", "
        << width << ", "
        << height;
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

    Box* box = new Box(x1 - 1, y1 - 1, x2 - 1, y2 - 1);
    Option<Box*> maybeBox = Option<Box*>(box);
    return maybeBox;
}
