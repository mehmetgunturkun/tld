#include "common/Arguments.hpp"
#include "core/Box.hpp"
#include "core/ScoredBox.hpp"

class TestScore: public Score {
    int* array;
public:
    TestScore() {
        array = new int[1000]();
    }

    ~TestScore() {
        delete[] array;
    }

    double getValue(int modelId) {
        return 0.0;
    }
};

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);

    string boxString = args.getString("box");

    for (int i = 0; i < 1; i++) {
        Option<Box*> maybeBox = Box::fromLine(boxString);
        if (maybeBox.isDefined()) {
            Box* box = maybeBox.get();

            ScoredBox* scoredBox = new ScoredBox(box);
            scoredBox->withScore("test", new TestScore());

            delete scoredBox;

            println("%d. %s", i, box->toString().c_str());

            delete box;
        } else {
            println("%s", ("There is no valid box in string: " + boxString).c_str());
        }
    }
}
