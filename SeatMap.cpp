#include "SeatMap.h"
#include <iostream>
#include <iomanip>

const std::string RESET = "\033[0m";
const std::string GREEN = "\033[32m";
const std::string RED = "\033[31m";
const std::string YELLOW = "\033[33m";

SeatMap::SeatMap(int rows, int perRow, std::pair<int, int> bizRows)
    : totalRows(rows), seatsPerRow(perRow), businessClassRows(bizRows) {
    seatOccupancy.assign(totalRows, std::vector<bool>(seatsPerRow, false));
    for (int i = 0; i < seatsPerRow; ++i)
        seatLabels.push_back('A' + i);
}

bool SeatMap::isSeatAvailable(int row, int seat) const {
    if (row < 1 || row > totalRows || seat < 0 || seat >= seatsPerRow)
        return false;
    return !seatOccupancy[row - 1][seat];
}

void SeatMap::markSeatOccupied(int row, int seat) {
    if (row >= 1 && row <= totalRows && seat >= 0 && seat < seatsPerRow)
        seatOccupancy[row - 1][seat] = true;
}

std::string SeatMap::getSeatCode(int row, int seat) const {
    return std::to_string(row) + seatLabels[seat];
}



void SeatMap::display() const {
    std::cout << "\n=== Схема салона ===\n";
    if (businessClassRows.first <= businessClassRows.second) {
        std::cout << YELLOW << "Бизнес-класс: ряды " << businessClassRows.first
            << "–" << businessClassRows.second << RESET << "\n";
    }
    for (int r = 1; r <= totalRows; ++r) {
        std::cout << "[" << std::setw(2) << r << "]";
        for (int s = 0; s < seatsPerRow; ++s) {
            std::string code = getSeatCode(r, s);
            if (isSeatAvailable(r, s)) {
                std::cout << GREEN << code << RESET << " ";
            }
            else {
                std::cout << RED << code << RESET << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n" << GREEN << "Зелёный" << RESET << " — свободно, "
        << RED << "Красный" << RESET << " — занято\n\n";
}
