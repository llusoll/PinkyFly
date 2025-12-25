#ifndef SEATMAP_H
#define SEATMAP_H

#include <vector>
#include <string>
#include <stdexcept>
#include <utility> // для std::pair
#include "SeatConfig.h" 
class SeatMap {
public:
    int totalRows;
    int seatsPerRow;
    std::vector<std::vector<bool>> seatOccupancy;
    std::vector<char> seatLabels;
    std::pair<int, int> businessClassRows;

    static int totalSeatMapsCreated;

    SeatMap(int rows, int perRow, std::pair<int, int> bizRows);
    SeatMap(const SeatMap& other);
    SeatMap& operator=(const SeatMap& other);

    bool isSeatAvailable(int row, int seat) const;
    void markSeatOccupied(int row, int seat);
    std::string getSeatCode(int row, int seat) const;

    // ИЗМЕНЕННЫЙ МЕТОД — использует compile-time конфигурацию
    double getSeatPrice(int row, double baseEco, double baseBiz) const {
        return defaultSeatLayout.getPrice(row, baseEco, baseBiz);
    }

    void display() const;
    void validateCoordinates(int row, int seat) const;

    bool operator==(const SeatMap& other) const;
    bool operator!=(const SeatMap& other) const;

    friend std::ostream& operator<<(std::ostream& os, const SeatMap& sm);
};

#endif
