#include "Flight.h"
#include "Aircraft.h"  
#include "Booking.h"    
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

int Flight::totalFlights = 0;

void Flight::validateDateTime(const std::string& dt) {
    if (dt.size() < 16 || dt[4] != '-' || dt[7] != '-' || dt[10] != ' ' || dt[13] != ':') {
        throw std::invalid_argument("Неверный формат даты и времени. Ожидается ГГГГ-ММ-ДД ЧЧ:ММ");
    }
}

Flight::Flight(const std::string& fn, const std::string& tail,
    const std::string& dep, const std::string& arr,
    const std::string& schedDep, const std::string& schedArr,
    double eco, double biz)
    : flightNumber(fn), aircraftTail(tail), departureAirport(dep), arrivalAirport(arr),
    scheduledDeparture(schedDep), scheduledArrival(schedArr),
    status(FlightStatus::Scheduled),
    basePriceEconomy(eco), basePriceBusiness(biz) {
    validateDateTime(scheduledDeparture);
    validateDateTime(scheduledArrival);
    if (basePriceEconomy <= 0 || basePriceBusiness <= 0) {
        throw std::invalid_argument("Цены должны быть положительными.");
    }
    ++totalFlights;
}

// ✅ ОПРЕДЕЛИТЕ конструктор копирования и присваивание — вы их используете!
Flight::Flight(const Flight& other)
    : flightNumber(other.flightNumber), aircraftTail(other.aircraftTail),
    departureAirport(other.departureAirport), arrivalAirport(other.arrivalAirport),
    scheduledDeparture(other.scheduledDeparture), scheduledArrival(other.scheduledArrival),
    actualDeparture(other.actualDeparture), actualArrival(other.actualArrival),
    status(other.status), basePriceEconomy(other.basePriceEconomy),
    basePriceBusiness(other.basePriceBusiness) {
    ++totalFlights;
}

Flight& Flight::operator=(const Flight& other) {
    if (this == &other) return *this;
    flightNumber = other.flightNumber;
    aircraftTail = other.aircraftTail;
    departureAirport = other.departureAirport;
    arrivalAirport = other.arrivalAirport;
    scheduledDeparture = other.scheduledDeparture;
    scheduledArrival = other.scheduledArrival;
    actualDeparture = other.actualDeparture;
    actualArrival = other.actualArrival;
    status = other.status;
    basePriceEconomy = other.basePriceEconomy;
    basePriceBusiness = other.basePriceBusiness;
    return *this;
}

void Flight::delayFlight(const std::string& newTime) {
    if (this->status == FlightStatus::Canceled) {
        throw std::logic_error("Нельзя задержать отменённый рейс.");
    }
    validateDateTime(newTime);
    this->scheduledDeparture = newTime;
    this->status = FlightStatus::Delayed;
}

void Flight::updateStatus(FlightStatus st) {
    if (this->status == FlightStatus::Canceled && st != FlightStatus::Canceled) {
        throw std::logic_error("Отменённый рейс нельзя восстановить.");
    }
    this->status = st;
}

// ✅ ИСПРАВЛЕНО: захват targetTail по значению + правильный доступ к totalSeats
int Flight::getAvailableSeats(
    const std::vector<std::unique_ptr<Aircraft>>& aircrafts,
    const std::vector<std::unique_ptr<Booking>>& bookings
) const {
    std::string targetTail = this->aircraftTail; // ✅ локальная копия
    auto it = std::find_if(aircrafts.begin(), aircrafts.end(),
        [&](const auto& a) { return a->tailNumber == targetTail; });
    if (it == aircrafts.end()) {
        throw std::runtime_error("Самолёт с бортом " + aircraftTail + " не найден.");
    }
    int totalSeats = (*it)->totalSeats; // ✅ разыменование unique_ptr
    int booked = 0;
    for (const auto& b : bookings) {
        if (b->getFlightNumber() == this->flightNumber) {
            booked++;
        }
    }
    return totalSeats - booked;
}

// остальные методы — без изменений
std::string Flight::statusToString(FlightStatus s) {
    switch (s) {
    case FlightStatus::Scheduled: return "Запланирован";
    case FlightStatus::Departed: return "Вылетел";
    case FlightStatus::Delayed: return "Задержан";

    case FlightStatus::Arrived: return "Прибыл";
    case FlightStatus::Canceled: return "Отменён";
    default: return "Запланирован";
    }
}

FlightStatus Flight::stringToStatus(const std::string& s) {
    if (s == "Запланирован" || s == "Scheduled") return FlightStatus::Scheduled;
    if (s == "Вылетел" || s == "Departed") return FlightStatus::Departed;
    if (s == "Задержан" || s == "Delayed") return FlightStatus::Delayed;
    if (s == "Прибыл" || s == "Arrived") return FlightStatus::Arrived;
    if (s == "Отменён" || s == "Canceled") return FlightStatus::Canceled;
    return FlightStatus::Scheduled;
}

bool Flight::operator==(const Flight& other) const {
    return this->flightNumber == other.flightNumber;
}

bool Flight::operator!=(const Flight& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Flight& f) {
    os << "Рейс [" << f.flightNumber << " " << f.departureAirport << "→" << f.arrivalAirport
        << " Статус: " << Flight::statusToString(f.status) << "]";
    return os;
}



void Flight::loadAll(std::vector<std::unique_ptr<Flight>>& flights) {
    flights.clear();
    std::ifstream file("data/flights.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        line.pop_back();
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() < 11) continue;

        try {
            auto fl = std::make_unique<Flight>(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], std::stod(parts[9]), std::stod(parts[10]));
            fl->actualDeparture = parts[6];
            fl->actualArrival = parts[7];
            fl->status = stringToStatus(parts[8]);
            flights.push_back(std::move(fl));
        }
        catch (const std::exception& ex) {
            std::cerr << "Пропущен некорректный рейс:" << ex.what() << "\n";
        }
    }
    file.close();
}

void Flight::saveAll(const std::vector<std::unique_ptr<Flight>>& flights) {
    ensureDataDir();
    std::ofstream file("data/flights.txt");
    file << "# Flight;Aircraft;Dep;Arr;SchedDep;SchedArr;ActDep;ActArr;Status;EcoPrice;BizPrice\n";
    for (const auto& fl : flights) {
        file << fl->flightNumber << ";"
            << fl->aircraftTail << ";"
            << fl->departureAirport << ";"
            << fl->arrivalAirport << ";"
            << fl->scheduledDeparture << ";"
            << fl->scheduledArrival << ";"
            << fl->actualDeparture << ";"
            << fl->actualArrival << ";"
            << (fl->status == FlightStatus::Scheduled ? "Scheduled" :
                fl->status == FlightStatus::Departed ? "Departed" :
                fl->status == FlightStatus::Delayed ? "Delayed" :
                fl->status == FlightStatus::Arrived ? "Arrived" : "Canceled") << ";"
            << fl->basePriceEconomy << ";"
            << fl->basePriceBusiness << "\n";
    }
    file.close();
}
