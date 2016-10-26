#include <iostream>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;

int main(int argc, char** args) {

    vector<int> numbers = {7, 8, 9};
    numbers.insert(numbers.begin() + 3, 10);

    for (int i = 0; i < (int) numbers.size(); i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    cv::Mat src(20, 20, CV_8U);
    for (int i = 0; i < 20*20; ++i)
        src.at<uchar>(i) = (uchar) i;

    std::cout << src << std::endl;

    cv::Mat dst;
    cv::resize(src, dst, cv::Size(10, 10), cv::INTER_LINEAR);

    std::cout << dst << std::endl;
    return 0;
}
