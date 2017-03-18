#include "common/Colors.hpp"

Scalar Colors::RED = Scalar(0, 0, 255);
Scalar Colors::YELLOW = Scalar(0, 255, 255);
Scalar Colors::ORANGE = Scalar(0, 128, 255);
Scalar Colors::GREEN = Scalar(0, 255, 0);
Scalar Colors::PURPLE = Scalar(204, 0, 102);
Scalar Colors::BLUE = Scalar(255, 0, 0);

vector<Scalar> Colors::colors = {
    Colors::RED,
    Colors::BLUE,
    Colors::ORANGE,
    Colors::GREEN,
    Colors::PURPLE,
    Colors::YELLOW
};

void Colors::test() {
    cout << "OpenCV version : " << CV_VERSION << endl;
    cout << "Major version : " << CV_MAJOR_VERSION << endl;
    cout << "Minor version : " << CV_MINOR_VERSION << endl;
    cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;
}
