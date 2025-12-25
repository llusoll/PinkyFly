#include "Passenger.h"
#include <fstream>
#include "Utils.h"

Passenger::Passenger(const std::string& id, const std::string& fn, const std::string& ln,
    const std::string& dob, const std::string& pp, const std::string& phone,
    const std::string& em)
    : passengerId(id), firstName(fn), lastName(ln), dateOfBirth(dob),
    passportNumber(pp), contactPhone(phone), email(em) {}

void Passenger::updateContactInfo(const std::string& phone, const std::string& email) {
    contactPhone = phone;
    this->email = email;
}

bool Passenger::validatePassport() const {
    return passportNumber.size() >= 6 && passportNumber.size() <= 15;
}

void Passenger::loadAll(std::vector<Passenger>& passengers) {
    passengers.clear();
    std::ifstream file("data/passengers.txt");
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() != 7) continue;
        passengers.emplace_back(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6]);
    }
    file.close();
}

void Passenger::saveAll(const std::vector<Passenger>& passengers) {
    ensureDataDir();
    std::ofstream file("data/passengers.txt");
    file << "# ID;FirstName;LastName;DOB;Passport;Phone;Email\n";
    for (const auto& p : passengers) {
        file << p.passengerId << ";"
            << p.firstName << ";"
            << p.lastName << ";"
            << p.dateOfBirth << ";"
            << p.passportNumber << ";"
            << p.contactPhone << ";"
            << p.email << "\n";
    }
    file.close();
}