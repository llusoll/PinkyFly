#include "Airport.h"
#include <fstream>
#include "Utils.h"

Airport::Airport(const std::string& cd, const std::string& nm, const std::string& ct, const std::string& cn)
    : code(cd), name(nm), city(ct), country(cn) {}

void Airport::loadAll(std::vector<Airport>& airports) {
    airports.clear();
    std::ifstream file("data/airports.txt");
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() != 4) continue;
        airports.emplace_back(parts[0], parts[1], parts[2], parts[3]);
    }
    file.close();
}

void Airport::saveAll(const std::vector<Airport>& airports) {
    ensureDataDir();
    std::ofstream file("data/airports.txt");
    file << "# Code;Name;City;Country\n";
    for (const auto& a : airports) {
        file << a.code << ";"
            << a.name << ";"
            << a.city << ";"
            << a.country << "\n";
    }
    file.close();
}