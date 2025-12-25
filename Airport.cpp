#include "Airport.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

int Airport::totalAirports = 0;

void Airport::validate(const std::string& code, const std::string& name) {
    if (code.empty() || code.size() > 5) {
        throw std::invalid_argument("Код аэропорта должен быть 1–5 букв.");
    }
    if (name.empty()) {
        throw std::invalid_argument("Название аэропорта не может быть пустым.");
    }
    if (!std::all_of(code.begin(), code.end(), [](unsigned char c) { return std::isalpha(c); })) {
        throw std::invalid_argument("Код аэропорта должен содержать только буквы (например, SVO).");
    }
}

Airport::Airport(const std::string& cd, const std::string& nm, const std::string& ct, const std::string& cn)
    : code(cd), name(nm), city(ct), country(cn) {
    validate(code, name);
    ++totalAirports;
}

bool Airport::operator==(const Airport& other) const {
    return this->code == other.code;
}

bool Airport::operator!=(const Airport& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Airport& a) {
    os << "Аэропорт[" << a.code << " – " << a.name << ", " << a.city << ", " << a.country << "]";
    return os;
}

void Airport::loadAll(std::vector<std::unique_ptr<Airport>>& airports) {
    airports.clear();
    std::ifstream file("data/airports.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        line.pop_back();
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() != 4) continue;

        try {
            auto a = std::make_unique<Airport>(parts[0], parts[1], parts[2], parts[3]);
            airports.push_back(std::move(a));
        }
        catch (const std::exception& ex) {
            std::cerr << "Пропущен некорректный аэропорт:" << ex.what() << "\n";
        }
    }
    file.close();
}

void Airport::saveAll(const std::vector<std::unique_ptr<Airport>>& airports) {
    ensureDataDir();
    std::ofstream file("data/airports.txt");
    file << "# Code;Name;City;Country\n";
    for (const auto& a : airports) {
        file << a->code << ";"
            << a->name << ";"
            << a->city << ";"
            << a->country << "\n";
    }
    file.close();
}
