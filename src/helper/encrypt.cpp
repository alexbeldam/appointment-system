#include "helper/encrypt.hpp"

#include <sstream>
using namespace std;

char map(char c) {
    return '0' + (c + 10) % 79;
}

string encrypt(const string& pwd) {
    stringstream ss;

    for (const char c : pwd)
        ss << map(c);

    return ss.str();
}

bool check(const string& cypher, const string& pwd) {
    return cypher == encrypt(pwd);
}