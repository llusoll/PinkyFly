#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

std::vector<std::string> split(const std::string& s, char delimiter);
void ensureDataDir();
std::string simpleHash(const std::string& s);

#endif