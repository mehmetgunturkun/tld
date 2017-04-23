#include "common/string/StringStream.hpp"

StringStream::StringStream(string& str, char delim) {
    stream << str;
    delimiter = delim;
}

bool StringStream::hasNext() {
    return stream.good();
}

string StringStream::next() {
    getline(stream, buffer, delimiter);
    return buffer;
}
