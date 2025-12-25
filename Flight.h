#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <vector>
#include <memory>     // для unique_ptr
#include <stdexcept>

class Aircraft;
class Booking;

enum class FlightStatus { Scheduled, Departed, Delayed, Arrived, Canceled };

class Flight {
public:
    std::string flightNumber;
    std::string aircraftTail;
    std::string departureAirport;
    std::string arrivalAirport;
    std::string scheduledDeparture;  // YYYY-MM-DD HH:MM
    std::string scheduledArrival;
    std::string actualDeparture;
    std::string actualArrival;
    FlightStatus status;
    double basePriceEconomy;
    double basePriceBusiness;

    static int totalFlights;

    Flight() = default;
    Flight(const std::string& fn, const std::string& tail,
        const std::string& dep, const std::string& arr,
        const std::string& schedDep, const std::string& schedArr,
        double eco, double biz);

    Flight(const Flight& other);
    Flight& operator=(const Flight& other);
    void delayFlight(const std::string& newTime);
    void updateStatus(FlightStatus st);

   
    int getAvailableSeats(
        const std::vector<std::unique_ptr<Aircraft>>& aircrafts,
        const std::vector<std::unique_ptr<Booking>>& bookings
    ) const;

    static void validateDateTime(const std::string& dt);
    static std::string statusToString(FlightStatus s);
    static FlightStatus stringToStatus(const std::string& s);

    bool operator==(const Flight& other) const;
    bool operator!=(const Flight& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Flight& f);

    static void loadAll(std::vector<std::unique_ptr<Flight>>& flights);
    static void saveAll(const std::vector<std::unique_ptr<Flight>>& flights);
};

#endif 