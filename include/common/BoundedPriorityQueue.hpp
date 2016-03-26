#ifndef BOUNDED_PRIORITY_QUEUE_H
#define BOUNDED_PRIORITY_QUEUE_H

#define UNBOUNDED -1

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <queue>

template <class T, class V>
class BoundedPriorityQueue: public std::priority_queue<T*, vector<T*>, V> {
    int limit;
public:
    int size;
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

    vector<T*> toVector() {
        return this->c;
    }

    T* head() {
        return this->top();
    }
};
#endif
