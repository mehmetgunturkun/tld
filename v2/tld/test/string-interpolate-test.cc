
#include <iostream>
#include "boost/format.hpp"

using namespace std;
using boost::format;


int main(int argc, char** argv) {

    double a = 3.1234;
    cout << format("%4.3f") % a << endl;
    return 0;
}
