#include "common/Arguments.hpp"
#include "core/Sequence.hpp"
#include "core/Frame.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);
    Sequence sequence = Sequence(args.getString("sequence"));

    for (int i = 0; i < 1; i++) {
        Frame* frame = sequence.next();
        println("%s", frame->name.c_str());
        IplImage* originalImage = frame->originalImage;
        Mat original = cvarrToMat(originalImage);

        long total_red = 0;
        long total_blu = 0;
        long total_gre = 0;

        for (int i = 0; i < original.rows; i++) {
            for (int j = 0; j < original.cols; j++) {
                Vec3b rgb = original.at<Vec3b>(i, j);
                total_red += rgb[0];
                total_blu += rgb[1];
                total_gre += rgb[2];
            }
        }


        printf("Red: %10lu, Blue: %10lu, Green: %10lu\n", total_red, total_blu, total_gre);

        IplImage* gray = Image::bgr2gray(originalImage);
        Mat grayMat = cvarrToMat(gray);
        long total_gray = 0;

        for (int i = 0; i < grayMat.rows; i++) {
            for (int j = 0; j < grayMat.cols; j++) {
                uchar pixel = grayMat.at<uchar>(i, j);
                total_gray += pixel;
            }
        }
        printf("Gray: %10lu\n", total_gray);

        IplImage* grayscale = frame->grayscale;

        long total = 0;

        for (int i = 0; i < frame->height; i++) {
            for (int j = 0; j < frame->width; j++) {
                char pixel = grayscale->imageData[i * frame->width + j];
                total += (uchar) pixel;
            }
        }

        println("Total: %10lu", total);

        delete frame;
    }
}
