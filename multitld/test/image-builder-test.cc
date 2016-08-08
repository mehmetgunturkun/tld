#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "util/ImageBuilder.hpp"

int main(int argc, char** args) {
    Frame* coloredImage = new Frame("resources/colored-test.jpg");
    ImageBuilder* builder = new ImageBuilder(coloredImage->displayImg);

    builder->withBox(new Box(1, 50, 50, 100, 100));
    builder->withBox(new Box(2, 75, 75, 125, 125), Colors::YELLOW);


    Image::imshow("Frame", builder->img, 0);
}
