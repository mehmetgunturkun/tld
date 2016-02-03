#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>

#include <json/json.h>
#include <json/value.h>

int main(int argc, char** argv) {
    Json::Value fromScratch;
    Json::Value array;
    array.append("hello");
    array.append("world");
    fromScratch["hello"] = "world";
    fromScratch["number"] = 2;
    fromScratch["array"] = array;
    fromScratch["object"]["hello"] = "world";

    // write in a nice readible way
    Json::StyledWriter styledWriter;
    std::cout << styledWriter.write(fromScratch);

    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    std::ifstream test("fatTrack.json", std::ifstream::binary);

    reader.parse(test, root, false );

    std::cout << styledWriter.write(root);
    return 0;
}
