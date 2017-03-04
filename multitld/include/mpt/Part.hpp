#ifndef PART_H
#define PART_H

class Part {
public:
    string shortName;
    Box* box;
    double p;
    double theta;

    Part(string shortName, Box* root, Box* part);
};
#endif
