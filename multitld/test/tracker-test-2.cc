
#include "tracker/Tracker.hpp"
#include "testbase/Sequence.hpp"

int main(int argc, char** argv) {
    Tracker* tracker = new Tracker();
    Sequence* sequence = new Sequence("car");

    Frame* frame1 = sequence->get(19);
    Frame* frame2 = sequence->get(20);

    vector<tld::Point*> pointList = {
        new tld::Point(55.5505, 120.054),
        new tld::Point(64.3283, 120.054),
        new tld::Point(73.1061, 120.054),
        new tld::Point(81.8839, 120.054),
        new tld::Point(90.6616, 120.054),
        // new tld::Point(99.4394, 120.054),
        // new tld::Point(108.217, 120.054),
        // new tld::Point(116.995, 120.054),
        new tld::Point(125.773, 120.054),
        new tld::Point(134.551, 120.054),
        new tld::Point(55.5505, 123.165),
        new tld::Point(64.3283, 123.165),
        new tld::Point(73.1061, 123.165),
        new tld::Point(81.8839, 123.165),
        new tld::Point(90.6616, 123.165),
        new tld::Point(99.4394, 123.165),
        new tld::Point(108.217, 123.165),
        new tld::Point(116.995, 123.165),
        new tld::Point(125.773, 123.165),
        new tld::Point(134.551, 123.165),
        new tld::Point(55.5505, 126.277),
        new tld::Point(64.3283, 126.277),
        new tld::Point(73.1061, 126.277),
        new tld::Point(81.8839, 126.277),
        new tld::Point(90.6616, 126.277),
        new tld::Point(99.4394, 126.277),
        new tld::Point(108.217, 126.277),
        new tld::Point(116.995, 126.277),
        new tld::Point(125.773, 126.277),
        new tld::Point(134.551, 126.277),
        new tld::Point(55.5505, 129.388),
        new tld::Point(64.3283, 129.388),
        new tld::Point(73.1061, 129.388),
        new tld::Point(81.8839, 129.388),
        new tld::Point(90.6616, 129.388),
        new tld::Point(99.4394, 129.388),
        new tld::Point(108.217, 129.388),
        new tld::Point(116.995, 129.388),
        new tld::Point(125.773, 129.388),
        new tld::Point(134.551, 129.388),
        new tld::Point(55.5505, 132.499),
        new tld::Point(64.3283, 132.499),
        new tld::Point(73.1061, 132.499),
        new tld::Point(81.8839, 132.499),
        new tld::Point(90.6616, 132.499),
        new tld::Point(99.4394, 132.499),
        new tld::Point(108.217, 132.499),
        new tld::Point(116.995, 132.499),
        new tld::Point(125.773, 132.499),
        new tld::Point(134.551, 132.499),
        new tld::Point(55.5505, 135.61),
        new tld::Point(64.3283, 135.61),
        new tld::Point(73.1061, 135.61),
        new tld::Point(81.8839, 135.61),
        new tld::Point(90.6616, 135.61),
        new tld::Point(99.4394, 135.61),
        new tld::Point(108.217, 135.61),
        new tld::Point(116.995, 135.61),
        new tld::Point(125.773, 135.61),
        new tld::Point(134.551, 135.61),
        new tld::Point(55.5505, 138.721),
        new tld::Point(64.3283, 138.721),
        new tld::Point(73.1061, 138.721),
        new tld::Point(81.8839, 138.721),
        new tld::Point(90.6616, 138.721),
        new tld::Point(99.4394, 138.721),
        new tld::Point(108.217, 138.721),
        new tld::Point(116.995, 138.721),
        new tld::Point(125.773, 138.721),
        new tld::Point(134.551, 138.721),
        new tld::Point(55.5505, 141.832),
        new tld::Point(64.3283, 141.832),
        new tld::Point(73.1061, 141.832),
        new tld::Point(81.8839, 141.832),
        new tld::Point(90.6616, 141.832),
        new tld::Point(99.4394, 141.832),
        new tld::Point(108.217, 141.832),
        new tld::Point(116.995, 141.832),
        new tld::Point(125.773, 141.832),
        new tld::Point(134.551, 141.832),
        new tld::Point(55.5505, 144.943),
        new tld::Point(64.3283, 144.943),
        new tld::Point(73.1061, 144.943),
        new tld::Point(81.8839, 144.943),
        new tld::Point(90.6616, 144.943),
        new tld::Point(99.4394, 144.943),
        new tld::Point(108.217, 144.943),
        new tld::Point(116.995, 144.943),
        new tld::Point(125.773, 144.943),
        new tld::Point(134.551, 144.943),
        new tld::Point(55.5505, 148.054),
        new tld::Point(64.3283, 148.054),
        new tld::Point(73.1061, 148.054),
        new tld::Point(81.8839, 148.054),
        new tld::Point(90.6616, 148.054),
        new tld::Point(99.4394, 148.054),
        new tld::Point(108.217, 148.054),
        new tld::Point(116.995, 148.054),
        new tld::Point(125.773, 148.054),
        new tld::Point(134.551, 148.054)
    };

    // vector<tld::Point*> newPointList = tracker->lkTrack(frame1, frame2, pointList);
    //
    // cout << "------------------\n";
    // for (int i = 0; i < pointList.size(); i++) {
    //     tld::Point* p1 = pointList[i];
    //     tld::Point* p2 = newPointList[i];
    //     cout << p1->toString() << "\n";
    //     cout << p2->toString() << "\n";
    //     cout << "------------------\n";
    // }

    return EXIT_SUCCESS;
}
