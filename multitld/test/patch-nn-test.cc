
#include "testbase/Sequence.hpp"
#include "detector/nn/Patch.hpp"
#include "detector/nn/ObjectModel.hpp"

void printDoubleImage(Mat* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            double p = m->at<double>(i, j);
            printf("% 02.6g ", p);
        }
        printf("\n------------------------------------------------\n");
    }
}

int main() {
    Sequence* sequence = new Sequence("tld/car");
    Frame* f0 = sequence->next();
    Frame* f1 = sequence->next();

    Box* b0 = new Box(0, 141, 125, 231, 164);
    Box* b1 = new Box(1, 137.23, 124, 227, 163);

    Patch* p0 = new Patch(f0, b0);
    Patch* p1 = new Patch(f1, b1);

    ObjectModel* model = new ObjectModel();
    model->add(p0, true);

    // float score = model->computePositiveSimilarity(p1);
    // printf("Positive Score: %f\n", 1 - score);

}
