
#include "common/BoundedPriorityQueue.hpp"

class Integer {
public:
    int n;
    Integer(int n) {
        this->n = n;
    }
};


struct IntegerOrdered {
  bool operator() (Integer* x, Integer* y) {
      return x->n >= y->n;
  }
};

int main() {
    BoundedPriorityQueue<Integer, IntegerOrdered> queue = BoundedPriorityQueue<Integer, IntegerOrdered>(10);

    queue += new Integer(10);
    queue += new Integer(110);
    queue += new Integer(1012);
    queue += new Integer(3);
    queue += new Integer(12);
    queue += new Integer(122);
    queue += new Integer(1333);
    queue += new Integer(14444);
    queue += new Integer(155555);
    queue += new Integer(12131);
    queue += new Integer(1123123);
    queue += new Integer(54354351);
    queue += new Integer(13453465);
    queue += new Integer(11233123);
    queue += new Integer(543554351);
    queue += new Integer(134563465);

    vector<Integer*> numbers = queue.toVector();
    Integer* top = numbers[0];
    printf("Top: %d\n", top->n);


    for (int i = 0; i < numbers.size(); i++) {
        Integer* number = numbers[i];
        printf("%d ", number->n);
    }
    printf("\n");
}
