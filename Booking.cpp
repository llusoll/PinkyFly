
#include "Booking.h"
#include <fstream>
#include "Utils.h"

Booking::Booking(const std::string& pnr, const std::string& fn, const std::string& pid,
    const std::string& seat, BookingStatus st, double pr, const std::string& date)
    : bookingNumber(pnr), flightNumber(fn), passengerId(pid), seatNumber(seat), status(st), price(pr), bookingDate(date) {}

std::string Booking::statusToString(BookingStatus s) {
    switch (s) {
    case BookingStatus::Confirmed: return "Confirmed";
    case BookingStatus::Paid: return "Paid";
    case BookingStatus::Canceled: return "Canceled";
    default: return "Confirmed";
    }
}

BookingStatus Booking::stringToStatus(const std::string& s) {
    if (s == "Confirmed") return BookingStatus::Confirmed;
    if (s == "Paid") return BookingStatus::Paid;
    if (s == "Canceled") return BookingStatus::Canceled;
    return BookingStatus::Confirmed;
}

void Booking::cancelBooking() {
    status = BookingStatus::Canceled;
}

void Booking::confirmPayment() {
    status = BookingStatus::Paid;
}

void Booking::changeSeat(const std::string& newSeat) {
    seatNumber = newSeat;
}

void Booking::loadAll(std::vector<Booking>& bookings) {
    bookings.clear();
    std::ifstream file("data/bookings.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');

        if (parts.size() != 7) continue;
        Booking bk;
        bk.bookingNumber = parts[0];
        bk.flightNumber = parts[1];
        bk.passengerId = parts[2];
        bk.seatNumber = parts[3];
        bk.status = stringToStatus(parts[4]);
        bk.price = std::stod(parts[5]);
        bk.bookingDate = parts[6];
        bookings.push_back(bk);

    }
    file.close();
}



void Booking::saveAll(const std::vector<Booking>& bookings) {
    ensureDataDir();
    std::ofstream file("data/bookings.txt");
    file << "# PNR;Flight;Passenger;Seat;Status;Price;Date\n";
    for (const auto& b : bookings) {
        file << b.bookingNumber << ";"
            << b.flightNumber << ";"
            << b.passengerId << ";"
            << b.seatNumber << ";"
            << statusToString(b.status) << ";"
            << b.price << ";"
            << b.bookingDate << "\n";
    }
    file.close();
}

