
#include "common/Logging.hpp"
#include "core/Random.hpp"

int main(int argc, char** argv) {
    println("Test Case 1: Random shuffling...")

    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100};
    int nrOfNumbers = (int) numbers.size();
    for (int i = 0; i < nrOfNumbers; i++) {
        printf("%d ", numbers[i]);
    }
    println("")

    vector<int> shuffledNumbers = Random::randomSample(numbers);
    for (int i = 0; i < nrOfNumbers; i++) {
        printf("%d ", shuffledNumbers[i]);
    }

    println("")

}
