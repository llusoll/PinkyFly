#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <vector>

class Booking;

enum class FlightStatus { Scheduled, Departed, Delayed, Arrived, Canceled };

class Flight {
public:
    std::string flightNumber;
    std::string aircraftTail; 
    std::string departureAirport;
    std::string arrivalAirport;
    std::string scheduledDeparture;
    std::string scheduledArrival;
    std::string actualDeparture;
    std::string actualArrival;
    FlightStatus status;
    double basePriceEconomy;
    double basePriceBusiness;

    Flight() = default;
    Flight(const std::string& fn, const std::string& tail, const std::string& dep, const std::string& arr,
        const std::string& schedDep, const std::string& schedArr, double eco, double biz);

    static std::string statusToString(FlightStatus s);
    static FlightStatus stringToStatus(const std::string& s);
    void delayFlight(const std::string& newTime);
    void updateStatus(FlightStatus st);

    // ќбъ€вление метода Ч без реализации тела
    int getAvailableSeats(const std::vector<Booking>& bookings) const;

    static void loadAll(std::vector<Flight>& flights);
    static void saveAll(const std::vector<Flight>& flights);
};

#endif