#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <sstream>

using namespace std;

class StringUtils {
public:
    static vector<string> split(const std::string &s, char delim) {
        vector<string> elems;
        stringstream ss(s);
        string item;
        while (getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
};
#endif
