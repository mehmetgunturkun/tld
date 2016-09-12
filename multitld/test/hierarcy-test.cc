#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "util/ImageBuilder.hpp"
class BaseClass {
public:
    virtual void print();
    virtual BaseClass* merge(BaseClass* other);
};

class A: public BaseClass {
public:
    int attribute_a;

    A(int att_a) {
        this->attribute_a = att_a;
    }

    void print() {
        printf(ANSI_COLOR_RED    "Hello, I am A(%d)"    ANSI_COLOR_RESET "\n", attribute_a);
    }

    BaseClass* merge(BaseClass* other) {
        A* otherA = (A*) other;
        A* newA = new A(this->attribute_a + otherA->attribute_a);
        return newA;
    }
};

class B: public BaseClass {
public:
    float attribute_b;

    B(float att_b) {
        this->attribute_b = att_b;
    }

    void print() {
        printf(ANSI_COLOR_GREEN    "Hello, I am B(%f)"    ANSI_COLOR_RESET "\n", attribute_b);
    }

    BaseClass* merge(BaseClass* other) {
        B* otherB = (B*) other;
        B* newB = new B(this->attribute_b + otherB->attribute_b);
        return newB;
    }
};

int main(int argc, char** args) {
    BaseClass* a1 = new A(10);
    BaseClass* b1 = new B(3.14f);

    BaseClass* a2 = new A(20);
    BaseClass* b2 = new B(6.32f);

    BaseClass* a_prime = a1->merge(a2);
    BaseClass* b_prime = b1->merge(b2);

    a_prime->print();
    b_prime->print();
}
