
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <unordered_map>
using namespace std;

class DummyItem {

};

int main(int argc, char** argv) {
    unordered_map<string, DummyItem*> itemMap;

    itemMap["item2"] = new DummyItem();
    itemMap["item3"] = new DummyItem();
    itemMap["item4"] = new DummyItem();
    itemMap["item1"] = new DummyItem();

    for ( auto it = itemMap.begin(); it != itemMap.end(); ++it ) {
        DummyItem* item = it->second;
        delete item;
    }
}
