
#include "common/BoundedPriorityQueue.hpp"

// class Integer {
// public:
//     int n;
//     Integer(int n) {
//         this->n = n;
//     }
//
//     string toString() {
//         string str = "Integer";
//         return std;
//     }
// };
//
// class Node {
// public:
//     int id;
//     double data;
//
//     Node(double d) {
//         this->id = Node::getId();
//         this->data = d;
//     }
//
//     static int idGen;
//     static int getId() {
//         int nextId = Node::idGen;
//         Node::idGen += 1;
//         return nextId;
//     }
//
//     string toString() {
//         string str = "Integer";
//         return str;
//     }
// };
//
// int Node::idGen = 0;
//
// struct IntegerOrdered {
//   bool operator() (Integer* x, Integer* y) {
//       return x->n >= y->n;
//   }
// };
//
// struct NodeOrdered {
//     bool operator() (Node* x, Node* y) {
//         bool isBigger = x->data >= y->data;
//         return isBigger;
//     }
// };

int main() {
    // BoundedPriorityQueue<Integer, IntegerOrdered> queue = BoundedPriorityQueue<Integer, IntegerOrdered>(10);
    //
    // queue += new Integer(10);
    // queue += new Integer(110);
    // queue += new Integer(1012);
    // queue += new Integer(3);
    // queue += new Integer(12);
    // queue += new Integer(122);
    // queue += new Integer(1333);
    // queue += new Integer(14444);
    // queue += new Integer(155555);
    // queue += new Integer(12131);
    // queue += new Integer(1123123);
    // queue += new Integer(54354351);
    // queue += new Integer(13453465);
    // queue += new Integer(11233123);
    // queue += new Integer(543554351);
    // queue += new Integer(134563465);
    //
    // vector<Integer*> numbers = queue.toVector();
    // Integer* top = numbers[0];
    // printf("Top: %d\n", top->n);
    //
    // for (int i = 0; i < numbers.size(); i++) {
    //     Integer* number = numbers[i];
    //     printf("%d ", number->n);
    // }
    // printf("\n");
    //
    //
    // BoundedPriorityQueue<Node, NodeOrdered> nodeQueue = BoundedPriorityQueue<Node, NodeOrdered>(10);
    // nodeQueue += new Node(100.0);
    // nodeQueue += new Node(3.0);
    // nodeQueue += new Node(11.0);
    // nodeQueue += new Node(20.0);
    // nodeQueue += new Node(1.0);
    // nodeQueue += new Node(3.0);
    // nodeQueue += new Node(101232.0);


    // for (int i = 0; i < nodeQueue.count; i++) {
    //     Node* node = nodeQueue.top();
    //     nodeQueue.pop();
    //     printf("Node(%d, %g)\n", node->id, node->data);
    // }

    // vector<Node*> nodes = nodeQueue.toVector();
    // for (int i = 0; i < nodes.size(); i++) {
    //     Node* node = nodes[i];
    //     printf("Node(%d, %g)\n", node->id, node->data);
    // }
}
