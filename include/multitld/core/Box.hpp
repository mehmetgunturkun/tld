#ifndef BOX_H
#define BOX_H

class Box {
public:
    float x1;
    float y1;
    float x2;
    float y2;

    float width;
    float height;

    int nrOfPoints;

    Box(int id, float x1, float y1, float x2, float y2) {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;

        this->width = x2 - x1;
        this->height = y2 - y1;
    }
};
#endif
