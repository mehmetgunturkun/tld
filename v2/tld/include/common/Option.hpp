#ifndef OPTION_H
#define OPTION_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

template <typename Type>
class Option {
    Type item;
    bool empty;
public:
    Option(Type _item) {
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

    Type get() {
        if (!empty) {
            return item;
        } else {
            throw invalid_argument("None.get");
        }
    }
};
#endif
