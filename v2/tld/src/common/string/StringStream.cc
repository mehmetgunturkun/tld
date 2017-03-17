#include "common/string/StringStream.hpp"

StringStream::StringStream(string& str, char delim) {
    stream << str;
    delimiter = delim;
}

string StringStream::next() {
    getline(stream, buffer, delimiter);
    return buffer;
}
