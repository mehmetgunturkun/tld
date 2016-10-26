#ifndef BOUNDED_SORTED_VECTOR_H
#define BOUNDED_SORTED_VECTOR_H

#include "core/ScoredBox.hpp"

class BoundedSortedScoredBoxVector {
public:
    vector<ScoredBox*> underlying;
    int limit;
    int size;

    BoundedSortedScoredBoxVector(int l) {
        this->limit = l;
        this->size = 0;

        for (int i = 0; i < limit; i++) {
            underlying.push_back(NULL);
        }
    }

    void operator+=(ScoredBox* sb) {
        int index = -1;
        for (int i = 0; i < limit; i++) {
            ScoredBox* v = underlying[i];
            if (v == NULL || v->box->overlap < sb->box->overlap) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            if (size < limit) {
                underlying[index + 1] = underlying[index];
                underlying[index] = sb;
                size += 1;
            } else {
                underlying.insert(underlying.begin() + index, sb);
                underlying.pop_back();
            }
        }
    }
};
#endif
