#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class Passenger {
public:
    std::string passengerId;
    std::string firstName;
    std::string lastName;
    std::string dateOfBirth; // YYYY-MM-DD
    std::string passportNumber;
    std::string contactPhone;
    std::string email;

    static int totalPassengers;

    Passenger() = default;
    Passenger(const std::string& id, const std::string& fn, const std::string& ln, const std::string& dob, const std::string& pp, const std::string& phone, const std::string& em);
    Passenger(const Passenger& other) = delete;
    Passenger& operator=(const Passenger& other) = delete;

    void updateContactInfo(const std::string& phone, const std::string& email);

    static void validatePassport(const std::string& pp);
    static void validateEmail(const std::string& email);
    static void validatePhone(const std::string& phone);
    static void validateDate(const std::string& date);

    bool isValidPassport() const;

    bool operator==(const Passenger& other) const;
    bool operator!=(const Passenger& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Passenger& p);

    static void loadAll(std::vector<std::unique_ptr<Passenger>>& passengers);
    static void saveAll(const std::vector<std::unique_ptr<Passenger>>& passengers);
};

#endif
