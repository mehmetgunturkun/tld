#ifndef CODE_VECTOR_H
#define CODE_VECTOR_H

class CodeVector {
private:
    vector<int> codes;
public:
    int size;

    CodeVector(int nrOfBaseClassifiers) {
        codes.resize(nrOfBaseClassifiers);
        size = nrOfBaseClassifiers;
    }

    void set(int index, int binaryCode) {
        codes[index] = binaryCode;
    }

    int get(int index) {
        return codes[index];
    }
};

#endif
