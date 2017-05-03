#ifndef CODE_VECTOR_H
#define CODE_VECTOR_H

#include <vector>

using namespace std;

class CodeVector {
private:
public:
    int size;
    vector<int> codes;

    CodeVector(int nrOfBaseClassifiers) {
        codes.resize(nrOfBaseClassifiers);
        size = nrOfBaseClassifiers;
    }

    CodeVector(vector<int> codes) {
        this->size = (int) codes.size();
        this->codes = codes;
    }

    void set(int index, int binaryCode) {
        codes[index] = binaryCode;
    }

    int get(int index) {
        return codes[index];
    }
};

#endif
