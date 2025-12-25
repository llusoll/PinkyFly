#include "Aircraft.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

int Aircraft::totalAircraftCount = 0;

Aircraft::Aircraft(const std::string& tn, const std::string& mdl, int total, int biz, int eco)
    : tailNumber(tn), model(mdl), totalSeats(total), businessSeats(biz), economySeats(eco),
    status(AircraftStatus::InService), flightHours(0.0), maintenanceSchedule("") {
    validate();
    ++totalAircraftCount;
}

void Aircraft::validate() {
    if (tailNumber.empty()) {
        throw std::invalid_argument("Бортовой номер не может быть пустым.");
    }
    if (model.empty()) {
        throw std::invalid_argument("Модель не может быть пустой.");
    }
    if (totalSeats <= 0 || businessSeats < 0 || economySeats < 0) {
        throw std::invalid_argument("Количество мест должно быть положительным.");
    }
    if (businessSeats + economySeats != totalSeats) {
        throw std::invalid_argument("Сумма мест бизнеса и эконома должна равняться общему.");
    }
}

std::string Aircraft::statusToString(AircraftStatus s) {
    switch (s) {
    case AircraftStatus::InService: return "В эксплуатации";
    case AircraftStatus::Maintenance: return "На ТО";
    case AircraftStatus::Retired: return "Списан";
    default: return "В эксплуатации";
    }
}

AircraftStatus Aircraft::stringToStatus(const std::string& s) {
    if (s == "В эксплуатации" || s == "InService") return AircraftStatus::InService;
    if (s == "На ТО" || s == "Maintenance") return AircraftStatus::Maintenance;
    if (s == "Списан" || s == "Retired") return AircraftStatus::Retired;
    return AircraftStatus::InService;
}

void Aircraft::scheduleMaintenance(const std::string& date) {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
        throw std::invalid_argument("Дата ТО должна быть в формате ГГГГ-ММ-ДД.");
    }
    maintenanceSchedule = date;
    changeStatus(AircraftStatus::Maintenance);
}

void Aircraft::updateFlightHours(double hours) {
    if (hours < 0) {
        throw std::invalid_argument("Налёт не может быть отрицательным.");
    }
    flightHours += hours;
}

void Aircraft::changeStatus(AircraftStatus st) {
    status = st;
    if (this->status == AircraftStatus::Retired) {
        this->flightHours = 0.0;
    }
}

bool Aircraft::operator==(const Aircraft& other) const {
    return this->tailNumber == other.tailNumber;
}

bool Aircraft::operator!=(const Aircraft& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Aircraft& ac) {
    os << "Самолёт[Борт=" << ac.tailNumber << ", Модель=" << ac.model
        << ", Статус=" << Aircraft::statusToString(ac.status)
        << ", Налёт=" << ac.flightHours << "]";
    return os;
}

void Aircraft::loadAll(std::vector<std::unique_ptr<Aircraft>>& aircrafts) {
    aircrafts.clear();
    std::ifstream file("data/aircrafts.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        line.pop_back();
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() != 8) continue;

        try {
            auto ac = std::make_unique<Aircraft>();
            ac->tailNumber = parts[0];
            ac->model = parts[1];
            ac->totalSeats = std::stoi(parts[2]);
            ac->businessSeats = std::stoi(parts[3]);
            ac->economySeats = std::stoi(parts[4]);
            ac->status = stringToStatus(parts[5]);
            ac->maintenanceSchedule = parts[6];
            ac->flightHours = std::stod(parts[7]);
            ac->validate();
            aircrafts.push_back(std::move(ac));
        }
        catch (const std::exception& e) {
            std::cerr << "Пропущен некорректный самолёт:" << e.what() << "\n";
        }
    }
    file.close();
}

void Aircraft::saveAll(const std::vector<std::unique_ptr<Aircraft>>& aircrafts) {
    ensureDataDir();
    std::ofstream file("data/aircrafts.txt");
    file << "# Tail;Model;Total;Business;Economy;Status;Maintenance;FlightHours\n";
    for (const auto& ac : aircrafts) {
        file << ac->tailNumber << ";"
            << ac->model << ";"
            << ac->totalSeats << ";"
            << ac->businessSeats << ";"
            << ac->economySeats << ";"
            << (ac->status == AircraftStatus::InService ? "InService" :
                ac->status == AircraftStatus::Maintenance ? "Maintenance" : "Retired") << ";"
            << ac->maintenanceSchedule << ";"
            << ac->flightHours << "\n";
    }
    file.close();
}