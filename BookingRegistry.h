#ifndef BOOKINGREGISTRY_H
#define BOOKINGREGISTRY_H

#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include "Booking.h"

template<typename T>
class BookingRegistry {
    static_assert(std::is_base_of_v<Booking, T>, "T должен унаследовать Booking");

private:
    std::vector<std::unique_ptr<T>> bookings;

public:
    void add(std::unique_ptr<T> booking) {
        if (!booking) throw std::invalid_argument("Booking не может быть нулевым");
        bookings.push_back(std::move(booking));
    }

    void sortByPrice() {
        std::sort(bookings.begin(), bookings.end(),
            [](const auto& a, const auto& b) {
                return a->calculatePrice() < b->calculatePrice();
            });
    }

    T* findByPNR(const std::string& pnr) {
        auto it = std::find_if(bookings.begin(), bookings.end(),
            [&pnr](const auto& b) { return b->getBookingNumber() == pnr; });
        return (it != bookings.end()) ? it->get() : nullptr;
    }

    double getTotalRevenue() const {
        double total = 0.0;
        for (const auto& b : bookings) {
            if (b->getStatus() == BookingStatus::Paid) {
                total += b->calculatePrice();
            }
        }
        return total;
    }

    std::vector<const T*> getAll() const {
        std::vector<const T*> result;
        for (const auto& b : bookings) result.push_back(b.get());
        return result;
    }
};

#endif
