#include "core/Box.hpp"
#include "core/ScoredBox.hpp"
#include "core/BoxIterator.hpp"

#include "testbase/Sequence.hpp"
#include "common/BoundedSortedVector.hpp"

vector<Box*> getCombinedBoxList() {
    vector<Box*> boxList;
    boxList.push_back(new Box(0, 136.514, 124.388, 226.424, 163.349, 90.9105, 39.961));
    boxList.push_back(new Box(0, 131.753, 124.52, 221.805, 163.543, 91.052, 40.0225));
    boxList.push_back(new Box(0, 126.509, 124.029, 216.722, 163.123, 91.2131, 40.0938));
    boxList.push_back(new Box(0, 122.975, 124.522, 213.074, 163.566, 91.0995, 40.0437));
    boxList.push_back(new Box(0, 113.773, 122.479, 203.859, 161.517, 91.086, 40.038));
    boxList.push_back(new Box(0, 109.228, 121.454, 199.278, 160.476, 91.05, 40.0221));
    boxList.push_back(new Box(0, 105.659, 120.902, 195.742, 159.939, 91.0838, 40.0367));
    boxList.push_back(new Box(0, 100.399, 120.463, 190.464, 159.492, 91.0648, 40.0286));
    boxList.push_back(new Box(0, 94.6274, 119.795, 184.686, 158.821, 91.0582, 40.0257));
    boxList.push_back(new Box(0, 89.5423, 119.14, 179.598, 158.164, 91.0559, 40.0241));
    boxList.push_back(new Box(0, 85.5111, 117.49, 175.572, 156.517, 91.0608, 40.0267));
    boxList.push_back(new Box(0, 81.463, 117.488, 171.523, 156.515, 91.0599, 40.0267));
    boxList.push_back(new Box(0, 75.686, 116.324, 165.649, 155.307, 90.9631, 39.9833));
    boxList.push_back(new Box(0, 71.3968, 116.293, 161.475, 155.328, 91.0779, 40.0343));
    boxList.push_back(new Box(0, 66.1755, 115.051, 156.274, 154.094, 91.0988, 40.0433));
    boxList.push_back(new Box(0, 60.5678, 115.389, 150.576, 154.393, 91.0077, 40.0035));
    boxList.push_back(new Box(0, 54.443, 114.632, 144.47, 153.644, 91.0275, 40.0125));
    boxList.push_back(new Box(0, 49.4284, 114.191, 139.431, 153.192, 91.0025, 40.0009));
    boxList.push_back(new Box(0, 43.9749, 113.887, 134.007, 152.901, 91.032, 40.0141));
    boxList.push_back(new Box(0, 39.1049, 112.872, 129.029, 151.838, 90.9237, 39.9662));
    boxList.push_back(new Box(0, 32.4767, 113.537, 122.513, 152.552, 91.0363, 40.0157));
    boxList.push_back(new Box(0, 25.7244, 112.016, 115.807, 151.053, 91.0829, 40.0368));
    boxList.push_back(new Box(0, 20.17, 113.592, 110.197, 152.604, 91.0265, 40.0117));
    boxList.push_back(new Box(0, 14.4785, 113.84, 104.486, 152.843, 91.0078, 40.0036));
    boxList.push_back(new Box(0, 9.6394, 115.356, 99.6445, 154.357, 91.0051, 40.002));
    boxList.push_back(new Box(0, 5.02991, 116.604, 95.2065, 155.682, 91.1766, 40.0779));
    boxList.push_back(new Box(0, -9.33536, 110.406, 80.0058, 149.116, 90.3412, 39.71));
    boxList.push_back(new Box(0, 0.843288, 109.807, 87.529, 147.278, 87.6857, 38.4712));
    boxList.push_back(new Box(0, 5.24275, 108.167, 92.9182, 146.086, 88.6755, 38.9189));
    boxList.push_back(new Box(0, 13.1676, 106.769, 99.356, 143.988, 87.1884, 38.2192));
    boxList.push_back(new Box(0, 15.4248, 105.597, 101.836, 142.921, 87.4115, 38.3233));
    boxList.push_back(new Box(0, 20.1389, 104.668, 107.267, 142.33, 88.1282, 38.6625));
    boxList.push_back(new Box(0, 26.0912, 104.156, 113.34, 141.868, 88.2488, 38.7116));
    boxList.push_back(new Box(0, 30.2657, 103.708, 116.327, 140.869, 87.061, 38.161));
    boxList.push_back(new Box(0, 36.9384, 102.538, 123.025, 139.714, 87.087, 38.1762));
    boxList.push_back(new Box(0, 41.8528, 102.448, 127.633, 139.477, 86.7802, 38.029));
    boxList.push_back(new Box(0, 46.1326, 101.985, 132.42, 139.254, 87.287, 38.2682));
    boxList.push_back(new Box(0, 51.7763, 102.079, 138.346, 139.478, 87.5701, 38.3986));
    boxList.push_back(new Box(0, 56.1685, 102.592, 141.994, 139.642, 86.8253, 38.0508));
    boxList.push_back(new Box(0, 60.4913, 103.495, 146.016, 140.406, 86.5248, 37.9108));
    boxList.push_back(new Box(0, 60.1591, 105.808, 145.3, 142.541, 86.1412, 37.7329));
    boxList.push_back(new Box(0, 62.0381, 107.711, 145.579, 143.696, 84.5411, 36.9854));
    boxList.push_back(new Box(0, 61.3256, 108.577, 145.484, 144.852, 85.1586, 37.2746));
    boxList.push_back(new Box(0, 61.6911, 110.076, 146.159, 146.493, 85.4677, 37.4172));
    boxList.push_back(new Box(0, 62.1209, 111.101, 146.408, 147.434, 85.287, 37.3329));
    boxList.push_back(new Box(0, 62.7529, 112.314, 147.283, 148.762, 85.53, 37.4478));
    boxList.push_back(new Box(0, 63.1119, 114.074, 148.867, 151.094, 86.7551, 38.0193));
    boxList.push_back(new Box(0, 62.6514, 114.933, 148.223, 151.867, 86.5713, 37.9337));
    boxList.push_back(new Box(0, 63.4776, 116.508, 148.7, 153.278, 86.222, 37.7704));
    boxList.push_back(new Box(0, 63.1715, 117.326, 148.041, 153.933, 85.8698, 37.6073));
    boxList.push_back(new Box(0, 62.2395, 118.233, 148.246, 155.37, 87.0065, 38.1371));
    boxList.push_back(new Box(0, 62.2943, 118.681, 147.834, 155.599, 86.5398, 37.9183));
    boxList.push_back(new Box(0, 62.8705, 120.19, 147.753, 156.801, 85.8826, 37.6107));
    boxList.push_back(new Box(0, 63.3905, 121.5, 148.17, 158.064, 85.7792, 37.5637));
    boxList.push_back(new Box(0, 62.8841, 122.055, 147.96, 158.758, 86.0759, 37.7028));
    boxList.push_back(new Box(0, 63.141, 123.539, 147.476, 159.897, 85.3353, 37.3573));
    boxList.push_back(new Box(0, 62.8732, 123.375, 146.811, 159.548, 84.9374, 37.1734));
    boxList.push_back(new Box(0, 63.2888, 124.314, 147.659, 160.689, 85.3704, 37.3752));
    boxList.push_back(new Box(0, 61.8128, 125.504, 146.505, 162.03, 85.6924, 37.526));
    boxList.push_back(new Box(0, 62.494, 125.861, 146.87, 162.237, 85.3756, 37.3757));
    boxList.push_back(new Box(0, 62.249, 127.372, 147.626, 164.214, 86.3766, 37.842));
    boxList.push_back(new Box(0, 62.1215, 128.443, 147.1, 165.099, 85.9785, 37.6554));
    boxList.push_back(new Box(0, 61.0917, 129.211, 146.198, 165.929, 86.1067, 37.7181));
    boxList.push_back(new Box(0, 61.9213, 129.995, 145.979, 166.22, 85.0573, 37.225));
    boxList.push_back(new Box(0, 60.977, 130.105, 144.869, 166.254, 84.8923, 37.1493));
    boxList.push_back(new Box(0, 61.2097, 131.738, 145.174, 167.922, 84.9643, 37.1832));
    boxList.push_back(new Box(0, 60.7317, 132.472, 143.525, 168.108, 83.7935, 36.6359));
    boxList.push_back(new Box(0, 60.7623, 134.002, 144.655, 170.152, 84.8929, 37.1497));
    boxList.push_back(new Box(0, 61.268, 134.007, 145.231, 170.193, 84.9635, 37.1857));
    boxList.push_back(new Box(0, 63.761, 135.492, 147.193, 171.425, 84.4323, 36.933));
    boxList.push_back(new Box(0, 65.4933, 136.487, 148.274, 172.119, 83.781, 36.6318));
    boxList.push_back(new Box(0, 68.3443, 136.844, 151.63, 172.708, 84.2853, 36.8638));
    return boxList;
}


int main(int argc, char** args) {
    Sequence* sequence = new Sequence("tld/car");
    Frame* firstFrame = sequence->next();
    Box* firstBox = sequence->initBox;

    vector<Box*> boxList = getCombinedBoxList();
    for (int bid = 0; bid < (int) boxList.size(); bid++) {
        Box* box = boxList[bid];

        BoxIterator* iterator = new BoxIterator(firstFrame, firstBox, 10, 24);

        int nrOfClosestBox = 0;
        BoundedSortedVector<ScoredBox, ScoredBoxOverlapOrdered> positiveQueue = BoundedSortedVector<ScoredBox, ScoredBoxOverlapOrdered>(10);

        while (iterator->hasNext()) {
            Box* nextBox = iterator->next();

            double overlap = Box::computeOverlap(nextBox, box);
            nextBox->overlap = overlap;
            ScoredBox* sb = new ScoredBox(nextBox);
            if (overlap > 0.6) {
                nrOfClosestBox += 1;
                positiveQueue += sb;
            }
        }
        printf("There are %d boxes has overlap > 0.6\n", nrOfClosestBox);

        vector<ScoredBox*> sbList = positiveQueue.toVector();
        for (int i = 0; i < (int) sbList.size(); i++) {
            ScoredBox* sb = sbList[i];
            printf("%s\n", sb->box->toCharArr());
        }
    }
}
