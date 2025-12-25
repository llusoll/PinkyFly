#ifndef ECONOMYBOOKING_H
#define ECONOMYBOOKING_H

#include "Booking.h"

class EconomyBooking : public Booking {
public:
    EconomyBooking(const std::string& pnr,
        const std::string& fn,
        const std::string& pid,
        const std::string& seat,
        BookingStatus st,
        double basePr,
        const std::string& date)
        : Booking(pnr, fn, pid, seat, st, basePr, date) {
        priceMultiplier = 1.0;
    }

    EconomyBooking(const EconomyBooking& other)
        : Booking(other.bookingNumber, other.flightNumber, other.passengerId,
            other.seatNumber, other.status, other.basePrice, other.bookingDate) {
        priceMultiplier = other.priceMultiplier;
    }

    std::unique_ptr<Booking> clone() const override {
        return std::make_unique<EconomyBooking>(*this);
    }

    double calculatePrice() const override {
        return basePrice * priceMultiplier;
    }

    std::string getDescription() const override {
        return "Эконом-класс";
    }

    void applyDiscount(double percent) {
        if (percent < 0 || percent > 100)
            throw std::invalid_argument("Скидка должна быть от 0 до 100%");
        basePrice *= (1.0 - percent / 100.0);
    }
};
