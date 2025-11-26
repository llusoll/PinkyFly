#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include <vector>

enum class AircraftStatus { InService, Maintenance, Retired };

class Aircraft {
public:
    std::string tailNumber;
    std::string model;
    int totalSeats;
    int businessSeats;
    int economySeats;
    AircraftStatus status;
    std::string maintenanceSchedule; // YYYY-MM-DD
    double flightHours;

    Aircraft() = default;
    Aircraft(const std::string& tn, const std::string& mdl, int total, int biz, int eco);

    static std::string statusToString(AircraftStatus s);
    static AircraftStatus stringToStatus(const std::string& s);
    void scheduleMaintenance(const std::string& date);
    void updateFlightHours(double hours);
    void changeStatus(AircraftStatus st);

    static void loadAll(std::vector<Aircraft>& aircrafts);
    static void saveAll(const std::vector<Aircraft>& aircrafts);
};

#endif