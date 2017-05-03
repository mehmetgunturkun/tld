#ifndef BOX_HULL_H
#define BOX_HULL_H

#include "core/Box.hpp"

class BoxHull {
private:
    int size;
public:
    double x1;
    double y1;
    double x2;
    double y2;

    BoxHull() {
        this->size = 0;
        this->x1 = 0.0;
        this->y1 = 0.0;
        this->x2 = 0.0;
        this->y2 = 0.0;
    }

    void add(Box* b) {
        if (this->size == 0) {
            this->x1 = b->x1;
            this->y1 = b->y1;
            this->x2 = b->x2;
            this->y2 = b->y2;
        } else {
            if (b->x1 < this->x1) {
                this->x1 = b->x1;
            }

            if (b->y1 < this->y1) {
                this->y1 = b->y1;
            }

            if (b->x2 > this->x2) {
                this->x2 = b->x2;
            }

            if (b->y2 > this->y2) {
                this->y2 = b->y2;
            }
        }
        this->size += 1;
    }

    Box* getBox() {
        return new Box(0, x1, y1, x2, y2);
    }
};
#endif
