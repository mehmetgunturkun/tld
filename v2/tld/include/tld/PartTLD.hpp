#ifndef PART_TLD_H
#define PART_TLD_H

#include <vector>

#include "core/Box.hpp"
#include "core/Frame.hpp"
#include "tld/TLD.hpp"

using namespace std;

class Shape {
public:
    vector<Box*> parts;

    Shape(vector<Box*> parts);
};

class Orientation {
public:
    double p;
    double theta;

    Point2f* computeTheVotingCenter(Box* box);
};

class PartTLD {
private:
    int nrOfParts;
    TLD* tld;

    unordered_map<int, Orientation*> orientations;

    void voteForShape(Shape* shape, Box* part, Orientation* orientation);
    Shape* createShape(int nrOfParts, vector<Box*> parts);
public:
    PartTLD();

    Shape* init(Frame* frame, Box* initBox);
    Shape* track(Frame* prev, Frame* current, Shape* shape0);
};

#endif
