#include <vector>
#include <string>
#include <sstream>
using namespace std;

#define UNLIMITED -1

template <class T, class Compare>
class BoundedSortedVector: public vector<T*> {
private:
    Compare* comp;
    int getNextIndex(T* newItem) {
        for (int i = 0; i < this->count; i++) {
            T* item = this->at(i);
            bool res = comp->compare(newItem, item);
            if (res == true) {
                return i;
            }
        }
        return this->count;
    }
public:
    int limit;
    int count;
    BoundedSortedVector(int limit = UNLIMITED) {
        this->comp = new Compare();
        this->limit = limit;
        this->count = 0;
    }

    ~BoundedSortedVector() {
        delete this->comp;
    }


    void operator+=(T* item) {
        if (this->limit == UNLIMITED) {
            this->push_back(item);
        } else {
            int index = getNextIndex(item);
            this->insert(this->begin() + index, item);
            this->count += 1;

            if (this->count > this->limit) {
                T* lastItem = this->back();

                this->count -= 1;
                this->pop_back();

                delete lastItem;
            }
        }
    }

    vector<T*> toVector() {
        vector<T*> vect;
        for (int i = 0; i < this->count; i++) {
            T* item = this->at(i);
            if (item != nullptr) {
                vect.push_back(item);
            }
        }
        return vect;
    }
};
