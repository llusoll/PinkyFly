#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

enum class AircraftStatus { InService, Maintenance, Retired };

class Aircraft {
public:
    std::string tailNumber;
    std::string model;
    int totalSeats;
    int businessSeats;
    int economySeats;
    AircraftStatus status;
    std::string maintenanceSchedule;
    double flightHours;

    static int totalAircraftCount;

    Aircraft() = default;
    Aircraft(const std::string& tn, const std::string& mdl, int total, int biz, int eco);
    Aircraft(const Aircraft& other) = delete;
    Aircraft& operator=(const Aircraft& other) = delete;

    static std::string statusToString(AircraftStatus s);
    static AircraftStatus stringToStatus(const std::string& s);

    void scheduleMaintenance(const std::string& date);
    void updateFlightHours(double hours);
    void changeStatus(AircraftStatus st);

    void validate();

    bool operator==(const Aircraft& other) const;
    bool operator!=(const Aircraft& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Aircraft& ac);

    static void loadAll(std::vector<std::unique_ptr<Aircraft>>& aircrafts);
    static void saveAll(const std::vector<std::unique_ptr<Aircraft>>& aircrafts);
};

#endif