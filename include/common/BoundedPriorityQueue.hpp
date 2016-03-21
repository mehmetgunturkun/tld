#ifndef BOUNDED_PRIORITY_QUEUE_H
#define BOUNDED_PRIORITY_QUEUE_H

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <queue>

template <class T, class V>
class BoundedPriorityQueue: public std::priority_queue<T*, vector<T*>, V> {
    int limit;
    int size;
public:
    BoundedPriorityQueue(int l) {
        limit = l;
        size = 0;
    }

    void operator += (T* item) {
        if (size < limit) {
            this->push(item);
        } else {
            T* head = this->top();
            if (this->comp(item, head)) {
                this->pop();
                this->push(item);
            }
        }
    }
};
#endif
