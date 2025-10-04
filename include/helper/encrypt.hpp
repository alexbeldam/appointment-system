#ifndef ENCRYPT_HPP
#define ENCRYPT_HPP

#include <string>

std::string encrypt(const std::string& pwd);

bool check(const std::string& cypher, const std::string& pwd);

#endif