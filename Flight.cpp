#include "Flight.h"
#include "Booking.h" 
#include "Utils.h"
#include <fstream>
#include <algorithm>

Flight::Flight(const std::string& fn, const std::string& tail, const std::string& dep, const std::string& arr,
    const std::string& schedDep, const std::string& schedArr, double eco, double biz)
    : flightNumber(fn), aircraftTail(tail), departureAirport(dep), arrivalAirport(arr),
    scheduledDeparture(schedDep), scheduledArrival(schedArr),
    status(FlightStatus::Scheduled), basePriceEconomy(eco), basePriceBusiness(biz) {}

std::string Flight::statusToString(FlightStatus s) {
    switch (s) {
    case FlightStatus::Scheduled: return "Scheduled";
    case FlightStatus::Departed: return "Departed";
    case FlightStatus::Delayed: return "Delayed";
    case FlightStatus::Arrived: return "Arrived";
    case FlightStatus::Canceled: return "Canceled";
    default: return "Scheduled";
    }
}

FlightStatus Flight::stringToStatus(const std::string& s) {
    if (s == "Scheduled") return FlightStatus::Scheduled;
    if (s == "Departed") return FlightStatus::Departed;
    if (s == "Delayed") return FlightStatus::Delayed;
    if (s == "Arrived") return FlightStatus::Arrived;
    if (s == "Canceled") return FlightStatus::Canceled;
    return FlightStatus::Scheduled;
}

void Flight::delayFlight(const std::string& newTime) {
    scheduledDeparture = newTime;
    status = FlightStatus::Delayed;
}

void Flight::updateStatus(FlightStatus st) {
    status = st;
}

// –еализаци€ Ч только в .cpp, после #include "Booking.hpp"
int Flight::getAvailableSeats(const std::vector<Booking>& bookings) const {
    int total = 180; // можно уточнить по aircraftTail
    int booked = 0;
    for (const auto& b : bookings) {
        if (b.flightNumber == flightNumber) {
            booked++;
        }
    }
    return total - booked;
}

void Flight::loadAll(std::vector<Flight>& flights) {
    flights.clear();
    std::ifstream file("data/flights.txt");
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() < 11) continue;
        Flight fl;
        fl.flightNumber = parts[0];
        fl.aircraftTail = parts[1];
        fl.departureAirport = parts[2];
        fl.arrivalAirport = parts[3];
        fl.scheduledDeparture = parts[4];
        fl.scheduledArrival = parts[5];
        fl.actualDeparture = parts[6];
        fl.actualArrival = parts[7];
        fl.status = stringToStatus(parts[8]);
        fl.basePriceEconomy = std::stod(parts[9]);
        fl.basePriceBusiness = std::stod(parts[10]);
        flights.push_back(fl);
    }
    file.close();
}

void Flight::saveAll(const std::vector<Flight>& flights) {
    ensureDataDir();
    std::ofstream file("data/flights.txt");
    file << "# Flight;Aircraft;Dep;Arr;SchedDep;SchedArr;ActDep;ActArr;Status;EcoPrice;BizPrice\n";
    for (const auto& fl : flights) {
        file << fl.flightNumber << ";"
            << fl.aircraftTail << ";"
            << fl.departureAirport << ";"
            << fl.arrivalAirport << ";"
            << fl.scheduledDeparture << ";"
            << fl.scheduledArrival << ";"
            << fl.actualDeparture << ";"
            << fl.actualArrival << ";"
            << statusToString(fl.status) << ";"
            << fl.basePriceEconomy << ";"
            << fl.basePriceBusiness << "\n";
    }
    file.close();
}