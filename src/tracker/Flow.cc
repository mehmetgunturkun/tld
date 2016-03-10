#include "tracker/Flow.hpp"


Flow::Flow(Frame* s,
    Frame* t,
    vector<tld::Point*> op,
    vector<tld::Point*> np) {
         source = s;
         target = t;
         oldPoints = op;
         newPoints = np;

         pointCount = (int) newPoints.size();
         vector<Displacement*> displacementList;
         for (int i = 0; i < pointCount; i++) {
             tld::Point* newPoint = newPoints[i];
             tld::Point* oldPoint = oldPoints[newPoint->id];
             Displacement* displacement = new Displacement(newPoint->id, oldPoint, newPoint);
             displacementList.push_back(displacement);
         }
 }
