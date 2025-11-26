#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include <vector>

class Airport {
public:
    std::string code;
    std::string name;
    std::string city;
    std::string country;

    Airport() = default;
    Airport(const std::string& cd, const std::string& nm, const std::string& ct, const std::string& cn);

    static void loadAll(std::vector<Airport>& airports);
    static void saveAll(const std::vector<Airport>& airports);
};

#endif