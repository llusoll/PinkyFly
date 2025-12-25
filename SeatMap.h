#ifndef SEATMAP_H
#define SEATMAP_H

#include <vector>
#include <string>
#include <utility>

class SeatMap {
public:
    int totalRows;
    int seatsPerRow;
    std::vector<std::vector<bool>> seatOccupancy;
    std::vector<char> seatLabels;
    std::pair<int, int> businessClassRows;

    SeatMap(int rows, int perRow, std::pair<int, int> bizRows);
    bool isSeatAvailable(int row, int seat) const;
    void markSeatOccupied(int row, int seat);
    std::string getSeatCode(int row, int seat) const;
    double getSeatPrice(int row, double baseEco, double baseBiz) const;
    void display() const;
};

#endif