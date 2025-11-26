#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <vector>

enum class BookingStatus { Confirmed, Paid, Canceled };

class Booking {
public:
    std::string bookingNumber;
    std::string flightNumber;
    std::string passengerId;
    std::string seatNumber;
    BookingStatus status;
    double price;
    std::string bookingDate;

    Booking() = default;
    Booking(const std::string& pnr, const std::string& fn, const std::string& pid,
        const std::string& seat, BookingStatus st, double pr, const std::string& date);

    static std::string statusToString(BookingStatus s);
    static BookingStatus stringToStatus(const std::string& s);
    void cancelBooking();
    void confirmPayment();
    void changeSeat(const std::string& newSeat);

    static void loadAll(std::vector<Booking>& bookings);
    static void saveAll(const std::vector<Booking>& bookings);
};

#endif