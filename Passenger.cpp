#include "Passenger.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

int Passenger::totalPassengers = 0;

void Passenger::validatePassport(const std::string& pp) {
    if (pp.size() < 6 || pp.size() > 15) {
        throw std::invalid_argument("Паспорт должен содержать 6–15 цифр.");
    }
    if (!std::all_of(pp.begin(), pp.end(), [](unsigned char c) { return std::isdigit(c); })) {
        throw std::invalid_argument("Паспорт должен содержать только цифры.");
    }
}

void Passenger::validateEmail(const std::string& email) {
    size_t at = email.find('@');
    size_t dot = email.find('.', at);
    if (at == std::string::npos || dot == std::string::npos || at >= dot) {
        throw std::invalid_argument("Неверный формат email.");
    }
}

void Passenger::validatePhone(const std::string& phone) {
    if (!((phone.substr(0, 2) == "+7" && phone.size() == 12) || (phone.substr(0, 1) == "8" && phone.size() == 11))) {
        throw std::invalid_argument("Телефон должен быть +7XXXXXXXXX или 8XXXXXXXXX.");
    }
}

void Passenger::validateDate(const std::string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        throw std::invalid_argument("Дата должна быть в формате ГГГГ-ММ-ДД.");
    }
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(date[i])) {
            throw std::invalid_argument("Дата должна содержать только цифры (ГГГГ-ММ-ДД).");
        }
    }
}

void Passenger::saveAll(const std::vector<std::unique_ptr<Passenger>>& passengers) {
    ensureDataDir();
    std::ofstream file("data/passengers.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл passengers.txt для записи.");
    }
    file << "# ID;First;Last;DOB;Passport;Phone;Email\n";
    for (const auto& p : passengers) {
        file << p->passengerId << ";"
            << p->firstName << ";"
            << p->lastName << ";"
            << p->dateOfBirth << ";"
            << p->passportNumber << ";"
            << p->contactPhone << ";"
            << p->email << "\n";
    }
    file.close();
}

Passenger::Passenger(const std::string& id, const std::string& fn, const std::string& ln, const std::string& dob, const std::string& pp, const std::string& phone, const std::string& em)
    : passengerId(id), firstName(fn), lastName(ln), dateOfBirth(dob), passportNumber(pp), contactPhone(phone), email(em) {
    validatePassport(passportNumber);
    validateEmail(email);
    validatePhone(contactPhone);
    validateDate(dateOfBirth);
    ++totalPassengers;
}





void Passenger::updateContactInfo(const std::string& phone, const std::string& em) {
    validatePhone(phone);
    validateEmail(em);
    this->contactPhone = phone;
    this->email = em;
}

bool Passenger::isValidPassport() const {
    return passportNumber.size() >= 6 && passportNumber.size() <= 15;
}

bool Passenger::operator==(const Passenger& other) const {
    return this->passengerId == other.passengerId;
}

bool Passenger::operator!=(const Passenger& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Passenger& p) {
    os << "Пассажир[ID=" << p.passengerId << ", ФИО=" << p.lastName << " " << p.firstName
        << ", Паспорт=" << p.passportNumber << "]";
    return os;
}

void Passenger::loadAll(std::vector<std::unique_ptr<Passenger>>& passengers) {
    passengers.clear();
    std::ifstream file("data/passengers.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        line.pop_back();
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() != 7) continue;

        try {
            auto p = std::make_unique<Passenger>(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6]);
            passengers.push_back(std::move(p));
        }
        catch (const std::exception& ex) {
            std::cerr << "Пропущен некорректный пассажир:" << ex.what() << "\n";
        }
    }
    file.close();
}
