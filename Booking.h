#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

enum class BookingStatus { Confirmed, Paid, Canceled };

class Booking {
protected:
    std::string bookingNumber;
    std::string flightNumber;
    std::string passengerId;
    std::string seatNumber;
    BookingStatus status;
    double basePrice;
    std::string bookingDate;
    double priceMultiplier = 1.0;

public:
    virtual ~Booking() = default;

    Booking(const std::string& pnr,
        const std::string& fn,
        const std::string& pid,
        const std::string& seat,
        BookingStatus st,
        double basePr,
        const std::string& date);

    static std::string statusToString(BookingStatus s);

    // ЗАПРЕЩАЕМ копирование базового класса
    Booking(const Booking&) = delete;
    Booking& operator=(const Booking&) = delete;

    // === АБСТРАКТНЫЕ МЕТОДЫ ===
    virtual double calculatePrice() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::unique_ptr<Booking> clone() const = 0;

    // === ВИРТУАЛЬНЫЙ метод с реализацией ===
    virtual void cancelBooking();

    // Геттеры (публичные)
    const std::string& getBookingNumber() const { return bookingNumber; }
    const std::string& getFlightNumber() const { return flightNumber; }
    const std::string& getPassengerId() const { return passengerId; }
    const std::string& getSeatNumber() const { return seatNumber; }
    BookingStatus getStatus() const { return status; }
    double getBasePrice() const { return basePrice; }
    const std::string& getBookingDate() const { return bookingDate; }

    friend std::ostream& operator<<(std::ostream& os, const Booking& b);

    static void loadAll(std::vector<std::unique_ptr<Booking>>& bookings);
    static void saveAll(const std::vector<std::unique_ptr<Booking>>& bookings);
};

#endif
