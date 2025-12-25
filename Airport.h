#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class Airport {
public:
    std::string code;
    std::string name;
    std::string city;
    std::string country;

    static int totalAirports;

    Airport() = default;
    Airport(const std::string& cd, const std::string& nm, const std::string& ct, const std::string& cn);
    Airport(const Airport& other) = delete;
    Airport& operator=(const Airport& other) = delete;

    static void validate(const std::string& code, const std::string& name);
    bool operator==(const Airport& other) const;
    bool operator!=(const Airport& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Airport& a);

    static void loadAll(std::vector<std::unique_ptr<Airport>>& airports);
    static void saveAll(const std::vector<std::unique_ptr<Airport>>& airports);
};

#endif
