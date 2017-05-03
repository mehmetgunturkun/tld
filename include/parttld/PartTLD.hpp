#ifndef PART_TLD_H
#define PART_TLD_H

#include <vector>

#include "core/Box.hpp"
#include "core/Frame.hpp"
#include "tld/TLD.hpp"

#include "parttld/VotingSpace.hpp"

using namespace std;

#define PI 3.14159265358979

class Shape {
public:
    vector<Box*> parts;
    Point2f* center;

    Shape(vector<Box*> parts);
};

class Orientation {
public:
    double p;
    double theta;

    Orientation(Box* center, Box* part);

    Point2f* computeTheVotingCenter(Box* box);
};

class PartTLD {
private:
    int nrOfParts;
    TLD* tld;

    unordered_map<int, Orientation*> orientations;

    void voteForShape(VotingSpace* space, Box* part, Orientation* orientation);
    Shape* createShape(int nrOfParts, vector<Box*> parts);
public:
    PartTLD();

    Shape* init(Frame* frame, Box* initBox);
    Shape* track(Frame* prev, Frame* current, Shape* shape0);
};

#endif
