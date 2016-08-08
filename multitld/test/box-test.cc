#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "util/ImageBuilder.hpp"

int main(int argc, char** args) {
    Frame* coloredImage = new Frame("resources/colored-test.jpg");
    ImageBuilder* builder = new ImageBuilder(coloredImage->displayImg);

    Box* mainBox = new Box(1, 10, 10, 210, 210);
    vector<Box*> subBoxes = mainBox->splitTwo();

    Box* b1 = subBoxes[0];
    Box* b2 = subBoxes[1];

    printf("BOX1 >> %s\n", b1->toString().c_str());
    printf("BOX2 >> %s\n", b2->toString().c_str());

    builder->withBox(mainBox, Colors::RED);
    builder->withBox(b1, Colors::YELLOW);
    builder->withBox(b2, Colors::YELLOW);
    builder->withTitle("Sub Box")->display(0);
}
