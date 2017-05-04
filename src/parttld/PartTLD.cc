#include "parttld/PartTLD.hpp"

Orientation::Orientation(Box* root, Box* part) {
    double rootX = (root->x1 + root->x2) / 2;
    double rootY = (root->y1 + root->y2) / 2;

    double partX = (part->x1 + part->x2) / 2;
    double partY = (part->y1 + part->y2) / 2;

    double dx = partX - rootX;
    double dy = partY - rootY;

    this->p = sqrt(dx * dx + dy * dy);
    this->theta = atan2(dy, dx) * 180.0 / PI;
}

Point2f* Orientation::computeTheVotingCenter(Box* box) {
    double mid_x = (box->x1 + box->x2) / 2;
    double mid_y = (box->y1 + box->y2) / 2;

    double x = mid_x + p * cos(theta);
    double y = mid_y + p * sin(theta);

    Point2f* center = new Point2f(x, y);
    return center;
}


Shape::Shape(vector<Box*> parts) {
    this->parts = parts;
}

PartTLD::PartTLD() {
    this->tld = new TLD();
}

Shape* PartTLD::init(Frame* frame, Box* initBox) {
    double x1 = initBox->x1;
    double y1 = initBox->y1;
    double x2 = initBox->x2;
    double y2 = initBox->y2;

    double mid_x = (x1 + x2) / 2;
    double mid_y = (y1 + y2) / 2;

    vector<Box*> parts = {
        new Box(0, x1, y1, mid_x, mid_y),
        new Box(1, mid_x, y1, x2, mid_y),
        new Box(2, x1, mid_y, mid_x, y2),
        new Box(3, mid_x, mid_y, x2, y2),
        new Box(4, (x1 + mid_x) / 2, (y1 + mid_y) / 2, (x2 + mid_x) / 2, (y2 + mid_x) / 2)
    };

    nrOfParts = (int) parts.size();

    vector<Box*> correctedParts = tld->init(frame, parts);
    for (int i = 0; i < nrOfParts; i++) {
        Box* correctedPart = correctedParts[i];
        Orientation* orientation_i = new Orientation(initBox, correctedPart);
        orientations[i] = orientation_i;
    }

    Shape* shape0 = new Shape(correctedParts);
    return shape0;
}

Shape* PartTLD::track(Frame* prev, Frame* current, Shape* shape0) {
    vector<Box*> parts0 = shape0->parts;
    vector<Box*> parts1 = tld->track(prev, current, parts0);
    Shape* shape1 = createShape(this->nrOfParts, parts1);
    return shape1;
}

Shape* PartTLD::createShape(int nrOfParts, vector<Box*> parts) {
    Shape* shape1 = new Shape(parts);
    VotingSpace* votingSpace = new VotingSpace();

    for (int i = 0; i < nrOfParts; i++) {
        Box* part_i = parts[i];
        if (part_i != nullptr) {
            Orientation* orientation_i = orientations[i];
            voteForShape(votingSpace, part_i, orientation_i);
        }
    }

    Point2f* center = votingSpace->result();
    shape1->center = center;
    return shape1;
}

void PartTLD::voteForShape(VotingSpace* votingSpace, Box* box, Orientation* orientation) {
    // Use the correct data structure
    Point2f* votingCenter = orientation->computeTheVotingCenter(box);

    int x = (int) votingCenter->x;
    int y = (int) votingCenter->y;

    for (int i = y - 50; i <  y + 50; i++) {
        for (int j = x - 50; j < x + 50; j++) {
            votingSpace->vote(j, i, 0);
        }
    }
}
