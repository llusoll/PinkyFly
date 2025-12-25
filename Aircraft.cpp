#include "Aircraft.h"
#include <fstream>
#include <iostream>
#include "Utils.h"

Aircraft::Aircraft(const std::string& tn, const std::string& mdl, int total, int biz, int eco)
    : tailNumber(tn), model(mdl), totalSeats(total), businessSeats(biz), economySeats(eco),
    status(AircraftStatus::InService), flightHours(0.0) {}

std::string Aircraft::statusToString(AircraftStatus s) {
    switch (s) {
    case AircraftStatus::InService: return "InService";
    case AircraftStatus::Maintenance: return "Maintenance";
    case AircraftStatus::Retired: return "Retired";
    default: return "InService";
    }
}

AircraftStatus Aircraft::stringToStatus(const std::string& s) {
    if (s == "InService") return AircraftStatus::InService;
    if (s == "Maintenance") return AircraftStatus::Maintenance;
    if (s == "Retired") return AircraftStatus::Retired;
    return AircraftStatus::InService;
}

void Aircraft::scheduleMaintenance(const std::string& date) {
    maintenanceSchedule = date;
}

void Aircraft::updateFlightHours(double hours) {
    flightHours += hours;
}

void Aircraft::changeStatus(AircraftStatus st) {
    status = st;
}

void Aircraft::loadAll(std::vector<Aircraft>& aircrafts) {
    aircrafts.clear();
    std::ifstream file("data/aircrafts.txt");
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() != 8) continue;
        Aircraft ac;
        ac.tailNumber = parts[0];
        ac.model = parts[1];
        ac.totalSeats = std::stoi(parts[2]);
        ac.businessSeats = std::stoi(parts[3]);
        ac.economySeats = std::stoi(parts[4]);
        ac.status = stringToStatus(parts[5]);
        ac.maintenanceSchedule = parts[6];
        ac.flightHours = std::stod(parts[7]);
        aircrafts.push_back(ac);
    }
    file.close();
}

void Aircraft::saveAll(const std::vector<Aircraft>& aircrafts) {
    ensureDataDir();
    std::ofstream file("data/aircrafts.txt");
    file << "# Tail;Model;Total;Business;Economy;Status;Maintenance;FlightHours\n";
    for (const auto& ac : aircrafts) {
        file << ac.tailNumber << ";"
            << ac.model << ";"
            << ac.totalSeats << ";"
            << ac.businessSeats << ";"
            << ac.economySeats << ";"
            << statusToString(ac.status) << ";"
            << ac.maintenanceSchedule << ";"
            << ac.flightHours << "\n";
    }
    file.close();
}