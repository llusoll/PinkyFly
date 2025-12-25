#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> split(const std::string& s, char delimiter);
void ensureDataDir();
std::string simpleHash(const std::string& s);

// Шаблонная функция: среднее арифметическое (только для арифметических типов)
template<typename T>
typename std::enable_if<std::is_arithmetic_v<T>, double>::type
safe_average(const std::vector<T>& values);
#endif
