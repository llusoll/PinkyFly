#include <vector>
#include <string>
#include <sstream>
#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#endif

std::vector<std::string> split(const std::string& s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}
void ensureDataDir() {
#ifdef  _WIN32
	_mkdir("data");
#else
	mkdir("data", 0755);
#endif
}
std::string simpleHash(const std::string& s) {
	const unsigned long long PRIME = 31;
	unsigned long long hash = 0;
	for (char c : s) {
		hash = hash * PRIME + static_cast<unsigned char>(c);
	}
	return std::to_string(hash);
}
// Реализация шаблонной функции
template<typename T>
typename std::enable_if<std::is_arithmetic_v<T>, double>::type
safe_average(const std::vector<T>& values) {
	if (values.empty()) return 0.0;
	double sum = 0.0;
	for (const auto& v : values) sum += static_cast<double>(v);
	return sum / values.size();
}

// Явная инстанциация
template double safe_average<int>(const std::vector<int>&);
template double safe_average<double>(const std::vector<double>&);
