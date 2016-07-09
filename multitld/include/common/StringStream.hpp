#ifndef STRING_STREAM_H
#define STRING_STREAM_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class StringStream {
private:
    stringstream stream;
    string buffer;
    char delimiter;
public:
    StringStream(string& str, char delim = ',') {
        stream << str;
        delimiter = delim;
    }

    string next() {
        getline(stream, buffer, delimiter);
        return buffer;
    }
};

#endif
