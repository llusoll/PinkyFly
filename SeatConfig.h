#ifndef SEAT_CONFIG_H
#define SEAT_CONFIG_H

struct SeatConfig {
    int totalRows;
    int seatsPerRow;
    int firstBusinessRow;
    int lastBusinessRow;

    // constexpr-конструктор
    constexpr SeatConfig(int rows, int perRow, int bizStart, int bizEnd)
        : totalRows(rows), seatsPerRow(perRow),
        firstBusinessRow(bizStart), lastBusinessRow(bizEnd) {
    }

    // constexpr-метод: проверка, является ли ряд бизнес-классом
    constexpr bool isBusiness(int row) const {
        return row >= firstBusinessRow && row <= lastBusinessRow;
    }

    // constexpr-метод: вычисление цены места
    constexpr double getPrice(int row, double eco, double biz) const {
        return isBusiness(row) ? biz : eco;
    }
};

constexpr SeatConfig defaultSeatLayout{ 30, 6, 1, 4 };

#endif
