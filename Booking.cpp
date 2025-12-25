
#include "Booking.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

Booking::Booking(const std::string& pnr,
    const std::string& fn,
    const std::string& pid,
    const std::string& seat,
    BookingStatus st,
    double basePr,
    const std::string& date)
    : bookingNumber(pnr), flightNumber(fn), passengerId(pid),
    seatNumber(seat), status(st), basePrice(basePr), bookingDate(date) {
}

void Booking::cancelBooking() {
    if (status == BookingStatus::Paid) {
        throw std::logic_error("Нельзя отменить оплаченное бронирование без возврата.");
    }
    status = BookingStatus::Canceled;
}

std::ostream& operator<<(std::ostream& os, const Booking& b) {
    os << "Бронь[" << b.bookingNumber
        << " | " << b.getDescription()
        << " | Цена: " << b.calculatePrice()
        << " | Статус: " << (b.status == BookingStatus::Confirmed ? "Подтверждено"
            : b.status == BookingStatus::Paid ? "Оплачено" : "Отменено")
        << "]";
    return os;
}

void Booking::loadAll(std::vector<std::unique_ptr<Booking>>& bookings) {
    bookings.clear();
    std::ifstream file("data/bookings.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() != 8) continue;

        try {
            std::string type = parts[0];
            auto pnr = parts[1];
            auto fn = parts[2];
            auto pid = parts[3];
            auto seat = parts[4];
            auto status = (parts[5] == "Confirmed") ? BookingStatus::Confirmed
                : (parts[5] == "Paid") ? BookingStatus::Paid
                : BookingStatus::Canceled;
            double basePrice = std::stod(parts[6]);
            auto date = parts[7];

            if (type == "ECONOMY") {
                bookings.push_back(std::make_unique<EconomyBooking>(
                    pnr, fn, pid, seat, status, basePrice, date));
            }
            else if (type == "BUSINESS") {
                bookings.push_back(std::make_unique<BusinessBooking>(
                    pnr, fn, pid, seat, status, basePrice, date));
            }
        }
        catch (const std::exception& ex) {
            std::cerr << "Пропущено: " << ex.what() << "\n";
        }
    }
    file.close();
}

void Booking::saveAll(const std::vector<std::unique_ptr<Booking>>& bookings) {
    ensureDataDir();
    std::ofstream file("data/bookings.txt");
    file << "# Type;PNR;Flight;Passenger;Seat;Status;BasePrice;Date\n";
    for (const auto& b : bookings) {
        std::string type = dynamic_cast<const EconomyBooking*>(b.get()) ? "ECONOMY" : "BUSINESS";
        file << type << ";"
            << b->getBookingNumber() << ";"
            << b->getFlightNumber() << ";"
            << b->getPassengerId() << ";"
            << b->getSeatNumber() << ";"
            << (b->getStatus() == BookingStatus::Confirmed ? "Confirmed"
                : b->getStatus() == BookingStatus::Paid ? "Paid" : "Canceled") << ";"
            << b->getBasePrice() << ";"
            << b->getBookingDate() << "\n";
    }
    file.close();
}
