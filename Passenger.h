#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <vector>

class Passenger {
public:
    std::string passengerId;
    std::string firstName;
    std::string lastName;
    std::string dateOfBirth;
    std::string passportNumber;
    std::string contactPhone;
    std::string email;

    Passenger() = default;
    Passenger(const std::string& id, const std::string& fn, const std::string& ln,
        const std::string& dob, const std::string& pp, const std::string& phone,
        const std::string& em);
    void updateContactInfo(const std::string& phone, const std::string& email);
    bool validatePassport() const;

    static void loadAll(std::vector<Passenger>& passengers);
    static void saveAll(const std::vector<Passenger>& passengers);
};

#endif