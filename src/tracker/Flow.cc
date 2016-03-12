#include "tracker/Flow.hpp"


Flow::Flow(Frame* s,
    Frame* t,
    vector<tld::Point*> op,
    vector<tld::Point*> np,
    vector<uchar> status,
    vector<float> errors) {
         source = s;
         target = t;

         oldPoints = op;
         newPoints = np;
         displacementCount = (int) oldPoints.size();
         for (int i = 0; i < displacementCount; i++) {
             tld::Point* srcPoint = oldPoints[i];
             tld::Point* targetPoint = newPoints[i];

             uchar state = status[i];
             float error = errors[i];

             Displacement* displacement = new Displacement(srcPoint->id,
                 srcPoint,
                 targetPoint,
                 state,
                 error
             );
             displacementList.push_back(displacement);
         }
 }
