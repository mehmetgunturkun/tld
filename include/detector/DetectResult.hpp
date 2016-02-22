#ifndef DETECTION_RESULT_HPP
#define DETECTION_RESULT_HPP

#include "core/Box.hpp"

using namespace std;

class DetectResult {
public:
    vector<Box*> all;
    vector<Box*> detected;
};
#endif
