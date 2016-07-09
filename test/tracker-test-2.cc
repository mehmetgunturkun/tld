#include "core/FrameView.hpp"
#include "util/Dataset.hpp"
#include "tracker/Tracker.hpp"

int main(int argc, char** argv) {
    Tracker* tracker = new Tracker();

    Dataset* dataset = new Dataset("car");
    Frame* firstFrame = dataset->get(19);
    Frame* secondFrame = dataset->get(20);

    vector<tld::Point*> pointList = {
        new tld::Point(1, 55.5505f, 120.054f),
        new tld::Point(1, 64.3283f, 120.054f),
        new tld::Point(1, 73.1061f, 120.054f),
        new tld::Point(1, 81.8839f, 120.054f),
        new tld::Point(1, 90.6616f, 120.054f),
        new tld::Point(1, 99.4394f, 120.054f),
        new tld::Point(1, 108.217f, 120.054f),
        new tld::Point(1, 116.995f, 120.054f),
        new tld::Point(1, 125.773f, 120.054f),
        new tld::Point(1, 134.551f, 120.054f),
        new tld::Point(1, 55.5505f, 123.165f),
        new tld::Point(1, 64.3283f, 123.165f),
        new tld::Point(1, 73.1061f, 123.165f),
        new tld::Point(1, 81.8839f, 123.165f),
        new tld::Point(1, 90.6616f, 123.165f),
        new tld::Point(1, 99.4394f, 123.165f),
        new tld::Point(1, 108.217f, 123.165f),
        new tld::Point(1, 116.995f, 123.165f),
        new tld::Point(1, 125.773f, 123.165f),
        new tld::Point(1, 134.551f, 123.165f),
        new tld::Point(1, 55.5505f, 126.277f),
        new tld::Point(1, 64.3283f, 126.277f),
        new tld::Point(1, 73.1061f, 126.277f),
        new tld::Point(1, 81.8839f, 126.277f),
        new tld::Point(1, 90.6616f, 126.277f),
        new tld::Point(1, 99.4394f, 126.277f),
        new tld::Point(1, 108.217f, 126.277f),
        new tld::Point(1, 116.995f, 126.277f),
        new tld::Point(1, 125.773f, 126.277f),
        new tld::Point(1, 134.551f, 126.277f),
        new tld::Point(1, 55.5505f, 129.388f),
        new tld::Point(1, 64.3283f, 129.388f),
        new tld::Point(1, 73.1061f, 129.388f),
        new tld::Point(1, 81.8839f, 129.388f),
        new tld::Point(1, 90.6616f, 129.388f),
        new tld::Point(1, 99.4394f, 129.388f),
        new tld::Point(1, 108.217f, 129.388f),
        new tld::Point(1, 116.995f, 129.388f),
        new tld::Point(1, 125.773f, 129.388f),
        new tld::Point(1, 134.551f, 129.388f),
        new tld::Point(1, 55.5505f, 132.499f),
        new tld::Point(1, 64.3283f, 132.499f),
        new tld::Point(1, 73.1061f, 132.499f),
        new tld::Point(1, 81.8839f, 132.499f),
        new tld::Point(1, 90.6616f, 132.499f),
        new tld::Point(1, 99.4394f, 132.499f),
        new tld::Point(1, 108.217f, 132.499f),
        new tld::Point(1, 116.995f, 132.499f),
        new tld::Point(1, 125.773f, 132.499f),
        new tld::Point(1, 134.551f, 132.499f),
        new tld::Point(1, 55.5505f, 135.61f),
        new tld::Point(1, 64.3283f, 135.61f),
        new tld::Point(1, 73.1061f, 135.61f),
        new tld::Point(1, 81.8839f, 135.61f),
        new tld::Point(1, 90.6616f, 135.61f),
        new tld::Point(1, 99.4394f, 135.61f),
        new tld::Point(1, 108.217f, 135.61f),
        new tld::Point(1, 116.995f, 135.61f),
        new tld::Point(1, 125.773f, 135.61f),
        new tld::Point(1, 134.551f, 135.61f),
        new tld::Point(1, 55.5505f, 138.721f),
        new tld::Point(1, 64.3283f, 138.721f),
        new tld::Point(1, 73.1061f, 138.721f),
        new tld::Point(1, 81.8839f, 138.721f),
        new tld::Point(1, 90.6616f, 138.721f),
        new tld::Point(1, 99.4394f, 138.721f),
        new tld::Point(1, 108.217f, 138.721f),
        new tld::Point(1, 116.995f, 138.721f),
        new tld::Point(1, 125.773f, 138.721f),
        new tld::Point(1, 134.551f, 138.721f),
        new tld::Point(1, 55.5505f, 141.832f),
        new tld::Point(1, 64.3283f, 141.832f),
        new tld::Point(1, 73.1061f, 141.832f),
        new tld::Point(1, 81.8839f, 141.832f),
        new tld::Point(1, 90.6616f, 141.832f),
        new tld::Point(1, 99.4394f, 141.832f),
        new tld::Point(1, 108.217f, 141.832f),
        new tld::Point(1, 116.995f, 141.832f),
        new tld::Point(1, 125.773f, 141.832f),
        new tld::Point(1, 134.551f, 141.832f),
        new tld::Point(1, 55.5505f, 144.943f),
        new tld::Point(1, 64.3283f, 144.943f),
        new tld::Point(1, 73.1061f, 144.943f),
        new tld::Point(1, 81.8839f, 144.943f),
        new tld::Point(1, 90.6616f, 144.943f),
        new tld::Point(1, 99.4394f, 144.943f),
        new tld::Point(1, 108.217f, 144.943f),
        new tld::Point(1, 116.995f, 144.943f),
        new tld::Point(1, 125.773f, 144.943f),
        new tld::Point(1, 134.551f, 144.943f),
        new tld::Point(1, 55.5505f, 148.054f),
        new tld::Point(1, 64.3283f, 148.054f),
        new tld::Point(1, 73.1061f, 148.054f),
        new tld::Point(1, 81.8839f, 148.054f),
        new tld::Point(1, 90.6616f, 148.054f),
        new tld::Point(1, 99.4394f, 148.054f),
        new tld::Point(1, 108.217f, 148.054f),
        new tld::Point(1, 116.995f, 148.054f),
        new tld::Point(1, 125.773f, 148.054f),
        new tld::Point(1, 134.551f, 148.054f)
    };

    Flow* flow = tracker->computeFlow(firstFrame, secondFrame, pointList);

    vector<Displacement*> displacementList = flow->displacementList;
    cout << "------------------\n";
    for (int i = 0; i < displacementList.size(); i++) {
        Displacement* d = displacementList[i];
        tld::Point* p1 = d->source;
        tld::Point* p2 = d->target;
        cout << p1->toString() << "\n";
        cout << p2->toString() << "\n";
        cout << "------------------\n";
    }
    return EXIT_SUCCESS;
}
