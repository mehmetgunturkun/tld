
#include "common/Config.hpp"

int main(int argc, char** argv) {
    Conf::load("/tmp/test.json");

    string a = Conf::getString("object.a");
    printf("object.a: %s\n", a.c_str());

    string d = Conf::getString("object.c");
    printf("object.c: %s\n", d.c_str());

    string f = Conf::getString("object.e");
    printf("object.e: %s\n", f.c_str());

    return 0;
}
