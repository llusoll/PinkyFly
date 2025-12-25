#ifndef COMPILE_TIME_UTILS_H
#define COMPILE_TIME_UTILS_H

#include <string>


constexpr bool isBusinessClassRow(int row, int firstBizRow, int lastBizRow) {
    return row >= firstBizRow && row <= lastBizRow;
}


constexpr double calculateSeatPriceConstEval(int row, double baseEco, double baseBiz,
    int firstBizRow, int lastBizRow) {
    return isBusinessClassRow(row, firstBizRow, lastBizRow) ? baseBiz : baseEco;
}

#endif 
