#include "core/Box.hpp"

Box::Box(int id, double x1, double y1, double x2, double y2) {

}

Box* Box::move(float dx, float dy) {
    Box* box = new Box(id, x1 + dx, y1 + dy, x2 + dx, y2 + dy);
    return box;
}
