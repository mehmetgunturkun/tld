#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {

    std::ofstream myfile;
    myfile.open ("/tmp/tldGate");
    int i = 0;
    while (i < 100000) {
        myfile << i << ". " << "Writing this to a file.\n";
        i++;
    }
    sleep(1);
    printf("END");
    myfile.flush();
    myfile.close();

    return 0;
}
