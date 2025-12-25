#ifndef BUSINESSBOOKING_H
#define BUSINESSBOOKING_H

#include "Booking.h"
#include <iostream>

class BusinessBooking : public Booking {
protected:
    bool hasPriorityBoarding = true;
    bool hasExtraBaggage = true;

public:
    BusinessBooking(const std::string& pnr,
        const std::string& fn,
        const std::string& pid,
        const std::string& seat,
        BookingStatus st,
        double basePr,
        const std::string& date)
        : Booking(pnr, fn, pid, seat, st, basePr, date) {
        priceMultiplier = 2.5;
    }

    BusinessBooking(const BusinessBooking& other)
        : Booking(other.bookingNumber, other.flightNumber, other.passengerId,
            other.seatNumber, other.status, other.basePrice, other.bookingDate),
        hasPriorityBoarding(other.hasPriorityBoarding),
        hasExtraBaggage(other.hasExtraBaggage) {
        priceMultiplier = other.priceMultiplier;
    }

    std::unique_ptr<Booking> clone() const override {
        return std::make_unique<BusinessBooking>(*this);
    }

    double calculatePrice() const override {
        return basePrice * priceMultiplier;
    }

    std::string getDescription() const override {
        return "Бизнес-класс (+багаж, приоритет)";
    }

    void cancelBooking() override {
        if (status == BookingStatus::Paid) {
            std::cout << "Возврат средств для бизнес-брони!\n";
        }
        Booking::cancelBooking();
    }

    BusinessBooking& operator=(const Booking& other) {
        if (this == &other) return *this;
        this->bookingNumber = other.getBookingNumber();
        this->flightNumber = other.getFlightNumber();
        this->passengerId = other.getPassengerId();
        this->seatNumber = other.getSeatNumber();
        this->status = other.getStatus();
        this->basePrice = other.getBasePrice();
        this->bookingDate = other.getBookingDate();
        // priceMultiplier остаётся 2.5
        return *this;
    }
};

#endif
