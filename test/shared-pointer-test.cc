#include <stdio.h>
#include <stdlib.h>
#include <memory>

using namespace std;

class InnerClass {
public:
    ~InnerClass() {
        printf("Destroying InnerClass\n");
    }
};

class OuterClass {
public:
        InnerClass* innerObject;

        OuterClass() {
            innerObject = new InnerClass();
        }

        ~OuterClass() {
            printf("Destroying OuterClass\n");
            delete innerObject;
        }
};

class InnerClass2 {
public:
    int item;
    InnerClass2(int i) {
        item = i;
    }
    ~InnerClass2() {
        printf("Destroying InnerClass2(%d)\n", item);
    }
};

class OuterClass2 {
public:
        shared_ptr<InnerClass2> innerObject;

        OuterClass2() {
            innerObject = make_shared<InnerClass2>(InnerClass2(43));
            innerObject->item = 100;
        }

        ~OuterClass2() {
            printf("Destroying OuterClass2\n");
        }
};


void createAndDestroyOuterObject() {
    OuterClass* outerObject = new OuterClass();
    delete outerObject;
}

shared_ptr<InnerClass2> createAndDestroyOuterObject2() {
    shared_ptr<OuterClass2> outerObject = make_shared<OuterClass2>(OuterClass2());
    shared_ptr<InnerClass2> innerObject = outerObject->innerObject;
    printf("1 >>> %d\n", innerObject->item);
    return innerObject;
}


int main() {
    createAndDestroyOuterObject();
    unique_ptr<InnerClass2> innerObj;
    innerObj.reset(new InnerClass2(1001));
    shared_ptr<InnerClass2> innerObject = createAndDestroyOuterObject2();
    printf("2 >>> %d\n", innerObject->item);
    return 0;
}
