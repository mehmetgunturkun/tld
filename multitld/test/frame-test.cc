#include "core/Frame.hpp"

int main(int argc, char** args) {
    // ------------------------------------------------------------------------------------------
    Frame* jpgImage = new Frame("resources/test.jpg");
    jpgImage->show();

    // ------------------------------------------------------------------------------------------

    Frame* coloredImage = new Frame("resources/colored-test.jpg");
    coloredImage->show();
    // ------------------------------------------------------------------------------------------
}
