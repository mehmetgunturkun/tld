#ifndef OPTION_H
#define OPTION_H
#include <stddef.h>

template <class Type>

class Option {
    Type* item;
    bool empty;
public:
    Option(Type* _item) {
        item = _item;
        empty = false;
    }
    Option() {
        empty =true;
    }
    bool isEmpty() {
        return empty;
    }
    bool isDefined() {
        return !isEmpty();
    }

    Type* get() {
        if (!empty) {
            return item;
        } else {
            throw 20;
        }
    }
};
#endif
