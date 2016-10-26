#include <vector>
#include "core/Box.hpp"
#include "core/Frame.hpp"

#include "testbase/Sequence.hpp"
#include "detector/nn/Patch.hpp"

void printPatch(Patch* p) {
    Mat* data = p->data;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            double pixel = data->at<double>(i, j);
            printf("%+.6g ", pixel);
        }
        printf("\n");
    }
}

double similarity(Patch* templatePatch, Patch* samplePatch) {
    Mat* templateImg = templatePatch->data;
    Mat* sampleImg = samplePatch->data;

    double corr = 0.0;
    double normTemplate = 0.0;
    double normSample = 0.0;

    for (int i = 0; i < Patch::HEIGHT; i++) {
        for (int j = 0; j < Patch::WIDTH; j++) {
            double templateVariance = templateImg->at<double>(i,j);
            double sampleVariance = sampleImg->at<double>(i,j);

            corr += templateVariance*sampleVariance;
            normTemplate += templateVariance*templateVariance;
            normSample += sampleVariance*sampleVariance;
        }
    }

    double upper = corr;
    double lower = sqrt(normTemplate * normSample);
    double ncc = (upper / lower);
    return (ncc + 1.0) / 2.0;
}

int main(int argc, char** args) {
    Sequence* sequence = new Sequence("tld/car");
    Frame* f0 = sequence->next();
    Box* b0 = new Box(0, 141, 125, 231, 164);
    Patch* p0 = new Patch(f0, b0);

    Frame* f1 = sequence->next();
    vector<Box*> boxList;
    boxList.push_back(new Box(13157, 133, 121, 223, 160));
    boxList.push_back(new Box(13158, 137, 121, 227, 160));
    boxList.push_back(new Box(13159, 141, 121, 231, 160));
    boxList.push_back(new Box(13212, 125, 125, 215, 164));
    boxList.push_back(new Box(13213, 129, 125, 219, 164));
    boxList.push_back(new Box(13214, 133, 125, 223, 164));
    boxList.push_back(new Box(13215, 137, 125, 227, 164));
    boxList.push_back(new Box(13216, 141, 125, 231, 164));
    boxList.push_back(new Box(13217, 145, 125, 235, 164));
    boxList.push_back(new Box(13272, 137, 129, 227, 168));
    int nrOfBoxList = (int) boxList.size();

    for (int i = 0; i < nrOfBoxList; i++) {
        Box* bi = boxList[i];
        Patch* pi = new Patch(f1, bi);
        printf("---------------------------\n");
        printf("%s\n", b0->toCharArr());
        printf("%s\n", bi->toCharArr());

        double similarityValue = similarity(p0, pi);
        printf("SIM: %g\n", similarityValue);
        printf("---------------------------\n");
        // printf("****************************\n");
        // printPatch(pi);
        // printf("****************************\n");
    }
}
