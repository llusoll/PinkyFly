#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include "Aircraft.h"
#include "Flight.h"
#include "Passenger.h"
#include "Booking.h"
#include "Employee.h"
#include "Airport.h"
#include "SeatMap.h"
#include "Utils.h"

// ---------- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ----------
std::string generatePNR() {
    static int counter = 1000;
    return "PNR" + std::to_string(++counter);
}

void clearScreen() {
    system("cls");
}

void pause() {
    std::cout << "\nНажмите Enter, чтобы продолжить...";
    std::cin.ignore();
    std::cin.get();
}

// Валидация по ТЗ
bool isValidPassport(const std::string& pp) {
    if (pp.size() < 6 || pp.size() > 15) return false;
    return std::all_of(pp.begin(), pp.end(), ::isdigit);
}

bool isValidEmail(const std::string& email) {
    size_t at = email.find('@');
    size_t dot = email.find('.', at);
    return at != std::string::npos && dot != std::string::npos && at < dot;
}

bool isValidPhone(const std::string& phone) {
    return (phone.substr(0, 2) == "+7" && phone.size() == 12) ||
        (phone.substr(0, 1) == "8" && phone.size() == 11);
}

bool isValidDate(const std::string& date) {
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(date[i])) return false;
    }
    return true;
}

// ---------- ТАБЛИЦЫ ----------
void displayTableAircraft(const std::vector<Aircraft>& list) {
    if (list.empty()) { std::cout << "Нет данных.\n"; return; }
    std::cout << "\n=== Воздушные суда ===\n";
    std::cout << std::left
        << std::setw(12) << "Борт"
        << std::setw(15) << "Модель"
        << std::setw(10) << "Всего"
        << std::setw(10) << "Бизнес"
        << std::setw(12) << "Статус"
        << std::setw(10) << "Налёт\n";
    for (const auto& a : list) {
        std::cout << std::left
            << std::setw(12) << a.tailNumber
            << std::setw(15) << a.model
            << std::setw(10) << a.totalSeats
            << std::setw(10) << a.businessSeats
            << std::setw(12) << Aircraft::statusToString(a.status)
            << std::setw(10) << a.flightHours << "\n";
    }
}

void displayTableFlights(const std::vector<Flight>& list) {
    if (list.empty()) { std::cout << "Нет данных.\n"; return; }
    std::cout << "\n=== Рейсы ===\n";
    std::cout << std::left
        << std::setw(10) << "Рейс"
        << std::setw(15) << "Маршрут"
        << std::setw(20) << "Вылет (расп.)"
        << std::setw(12) << "Статус\n";
    for (const auto& f : list) {
        std::cout << std::left
            << std::setw(10) << f.flightNumber
            << std::setw(15) << f.departureAirport + "->" + f.arrivalAirport
            << std::setw(20) << f.scheduledDeparture
            << std::setw(12) << Flight::statusToString(f.status) << "\n";
    }
}

void displayTablePassengers(const std::vector<Passenger>& list) {
    if (list.empty()) { std::cout << "Нет данных.\n"; return; }
    std::cout << "\n=== Пассажиры ===\n";
    std::cout << std::left
        << std::setw(15) << "Паспорт"
        << std::setw(20) << "ФИО"
        << std::setw(15) << "Телефон"
        << std::setw(20) << "Email\n";
    for (const auto& p : list) {
        std::cout << std::left
            << std::setw(15) << p.passportNumber
            << std::setw(20) << p.lastName + " " + p.firstName
            << std::setw(15) << p.contactPhone
            << std::setw(20) << p.email << "\n";
    }
}

void displayTableBookings(const std::vector<Booking>& list) {
    if (list.empty()) { std::cout << "Нет данных.\n"; return; }
    std::cout << "\n=== Бронирования ===\n";
    std::cout << std::left
        << std::setw(12) << "PNR"
        << std::setw(10) << "Рейс"
        << std::setw(15) << "Пассажир"
        << std::setw(8) << "Место"
        << std::setw(10) << "Статус\n";
    for (const auto& b : list) {
        std::cout << std::left
            << std::setw(12) << b.bookingNumber
            << std::setw(10) << b.flightNumber
            << std::setw(15) << b.passengerId
            << std::setw(8) << b.seatNumber
            << std::setw(10) << Booking::statusToString(b.status) << "\n";
    }
}

void displayTableEmployees(const std::vector<Employee>& list) {
    if (list.empty()) { std::cout << "Нет данных.\n"; return; }
    std::cout << "\n=== Сотрудники ===\n";
    std::cout << std::left
        << std::setw(10) << "ID"
        << std::setw(20) << "ФИО"
        << std::setw(15) << "Роль"
        << std::setw(15) << "Логин\n";
    for (const auto& e : list) {
        std::cout << std::left
            << std::setw(10) << e.employeeId
            << std::setw(20) << e.lastName + " " + e.firstName
            << std::setw(15) << Employee::roleToString(e.role)
            << std::setw(15) << e.login << "\n";
    }
}

void displayTableAirports(const std::vector<Airport>& list) {
    if (list.empty()) { std::cout << "Нет данных.\n"; return; }
    std::cout << "\n=== Аэропорты ===\n";
    std::cout << std::left
        << std::setw(8) << "Код"
        << std::setw(25) << "Название"
        << std::setw(15) << "Город"
        << std::setw(15) << "Страна\n";
    for (const auto& a : list) {
        std::cout << std::left
            << std::setw(8) << a.code
            << std::setw(25) << a.name
            << std::setw(15) << a.city
            << std::setw(15) << a.country << "\n";
    }
}

// ---------- МОДУЛИ ----------
void moduleAircraft(std::vector<Aircraft>& acs) {
    int choice;
    while (true) {
        clearScreen();
        displayTableAircraft(acs);
        std::cout << "\n1. Добавить ВС\n2. Редактировать ВС\n3. Удалить ВС\n4. Назад\n";
        std::cin >> choice;

        if (choice == 1) {
            std::string tn, mdl;
            int t, b, e;
            std::cout << "Бортовой номер: "; std::cin >> tn;
            std::cout << "Модель: "; std::cin >> mdl;
            std::cout << "Всего мест: "; std::cin >> t;
            std::cout << "Мест бизнес-класса: "; std::cin >> b;
            std::cout << "Мест эконом-класса: "; std::cin >> e;
            acs.emplace_back(tn, mdl, t, b, e);
            Aircraft::saveAll(acs);
        }
        else if (choice == 2) {
            std::string tn;
            std::cout << "Бортовой номер для редактирования: "; std::cin >> tn;
            auto it = std::find_if(acs.begin(), acs.end(), [&](const Aircraft& a) { return a.tailNumber == tn; });
            if (it != acs.end()) {
                std::cout << "Новая модель: "; std::cin >> it->model;
                std::cout << "Всего мест: "; std::cin >> it->totalSeats;
                std::cout << "Бизнес: "; std::cin >> it->businessSeats;
                std::cout << "Эконом: "; std::cin >> it->economySeats;
                std::cout << "Статус (InService/Maintenance/Retired): "; std::string st; std::cin >> st;
                it->status = Aircraft::stringToStatus(st);
                std::cout << "Дата ТО (YYYY-MM-DD): "; std::cin >> it->maintenanceSchedule;
                Aircraft::saveAll(acs);
            }
            else {
                std::cout << "ВС не найдено!\n"; pause();
            }
        }
        else if (choice == 3) {
            std::string tn;
            std::cout << "Бортовой номер для удаления: "; std::cin >> tn;
            acs.erase(std::remove_if(acs.begin(), acs.end(), [&](const Aircraft& a) { return a.tailNumber == tn; }), acs.end());
            Aircraft::saveAll(acs);
        }
        else if (choice == 4) break;
        else { std::cout << "Неверный выбор.\n"; pause(); }
    }
}

void moduleFlights(std::vector<Flight>& fls, const std::vector<Aircraft>& acs) {
    int choice;
    while (true) {
        clearScreen();
        displayTableFlights(fls);
        std::cout << "\n1. Добавить рейс\n2. Изменить статус\n3. Задержать рейс\n4. Удалить рейс\n5. Назад\n";
        std::cin >> choice;

        if (choice == 1) {
            std::string fn, tail, dep, arr, schedDep, schedArr;
            double eco, biz;
            std::cout << "Номер рейса: "; std::cin >> fn;
            std::cout << "Бортовой номер ВС: "; std::cin >> tail;
            if (std::find_if(acs.begin(), acs.end(), [&](const Aircraft& a) { return a.tailNumber == tail; }) == acs.end()) {
                std::cout << "ВС не найден!\n"; pause(); continue;
            }
            std::cout << "Аэропорт вылета (код): "; std::cin >> dep;
            std::cout << "Аэропорт прилёта (код): "; std::cin >> arr;
            std::cout << "Время вылета (YYYY-MM-DD HH:MM): "; std::cin >> schedDep;
            std::cout << "Время прилёта (YYYY-MM-DD HH:MM): "; std::cin >> schedArr;
            std::cout << "Базовая цена эконом: "; std::cin >> eco;
            std::cout << "Базовая цена бизнес: "; std::cin >> biz;
            fls.emplace_back(fn, tail, dep, arr, schedDep, schedArr, eco, biz);
            Flight::saveAll(fls);
        }
        else if (choice == 2) {
            std::string fn;
            std::cout << "Номер рейса: "; std::cin >> fn;
            auto it = std::find_if(fls.begin(), fls.end(), [&](const Flight& f) { return f.flightNumber == fn; });
            if (it != fls.end()) {
                std::cout << "Новый статус (Scheduled/Departed/Delayed/Arrived/Canceled): "; std::string st; std::cin >> st;
                it->status = Flight::stringToStatus(st);
                Flight::saveAll(fls);
            }
            else {
                std::cout << "Рейс не найден!\n"; pause();
            }
        }
        else if (choice == 3) {
            std::string fn, newTime;
            std::cout << "Номер рейса: "; std::cin >> fn;
            std::cout << "Новое время вылета (YYYY-MM-DD HH:MM): "; std::cin >> newTime;
            auto it = std::find_if(fls.begin(), fls.end(), [&](const Flight& f) { return f.flightNumber == fn; });
            if (it != fls.end()) {
                it->delayFlight(newTime);
                Flight::saveAll(fls);
            }
            else {
                std::cout << "Рейс не найден!\n"; pause();
            }
        }
        else if (choice == 4) {
            std::string fn;
            std::cout << "Номер рейса для удаления: "; std::cin >> fn;
            fls.erase(std::remove_if(fls.begin(), fls.end(), [&](const Flight& f) { return f.flightNumber == fn; }), fls.end());
            Flight::saveAll(fls);
        }
        else if (choice == 5) break;
        else { std::cout << "Неверный выбор.\n"; pause(); }
    }
}

void modulePassengers(std::vector<Passenger>& ps) {
    int choice;
    while (true) {
        clearScreen();
        displayTablePassengers(ps);
        std::cout << "\n1. Добавить пассажира\n2. Редактировать контактные данные\n3. Удалить пассажира\n4. Назад\n";
        std::cin >> choice;

        if (choice == 1) {
            std::string fn, ln, dob, pp, phone, email;
            std::cout << "Имя: "; std::cin >> fn;
            std::cout << "Фамилия: "; std::cin >> ln;
            std::cout << "Дата рождения (YYYY-MM-DD): "; std::cin >> dob;
            while (!isValidDate(dob)) { std::cout << "Неверный формат даты! Повторите: "; std::cin >> dob; }
            std::cout << "Паспорт (6-15 цифр): "; std::cin >> pp;
            while (!isValidPassport(pp)) { std::cout << "Неверный паспорт! Повторите: "; std::cin >> pp; }
            std::cout << "Телефон (+7... или 8...): "; std::cin >> phone;
            while (!isValidPhone(phone)) { std::cout << "Неверный телефон! Повторите: "; std::cin >> phone; }
            std::cout << "Email: "; std::cin >> email;
            while (!isValidEmail(email)) { std::cout << "Неверный email! Повторите: "; std::cin >> email; }
            std::string id = "P" + std::to_string(ps.size() + 1);
            ps.emplace_back(id, fn, ln, dob, pp, phone, email);
            Passenger::saveAll(ps);
        }
        else if (choice == 2) {
            std::string pp;
            std::cout << "Паспорт для редактирования: "; std::cin >> pp;
            auto it = std::find_if(ps.begin(), ps.end(), [&](const Passenger& p) { return p.passportNumber == pp; });
            if (it != ps.end()) {
                std::string phone, email;
                std::cout << "Новый телефон: "; std::cin >> phone;
                while (!isValidPhone(phone)) { std::cout << "Неверный телефон! Повторите: "; std::cin >> phone; }
                std::cout << "Новый email: "; std::cin >> email;
                while (!isValidEmail(email)) { std::cout << "Неверный email! Повторите: "; std::cin >> email; }
                it->updateContactInfo(phone, email);
                Passenger::saveAll(ps);
            }
            else {
                std::cout << "Пассажир не найден!\n"; pause();
            }
        }
        else if (choice == 3) {
            std::string pp;
            std::cout << "Паспорт для удаления: "; std::cin >> pp;
            ps.erase(std::remove_if(ps.begin(), ps.end(), [&](const Passenger& p) { return p.passportNumber == pp; }), ps.end());
            Passenger::saveAll(ps);
        }
        else if (choice == 4) break;
        else { std::cout << "Неверный выбор.\n"; pause(); }
    }
}

void moduleBookings(std::vector<Flight>& fls, std::vector<Passenger>& ps, std::vector<Booking>& bks) {
    if (fls.empty()) { std::cout << "Нет рейсов для бронирования!\n"; pause(); return; }
    displayTableFlights(fls);
    std::string fn;
    std::cout << "Введите номер рейса: "; std::cin >> fn;
    auto flIt = std::find_if(fls.begin(), fls.end(), [&](const Flight& f) { return f.flightNumber == fn; });
    if (flIt == fls.end()) { std::cout << "Рейс не найден!\n"; pause(); return; }

    int rows = 30, perRow = 6;
    int bizRows = (flIt->aircraftTail == "RA123") ? 4 : 2;
    SeatMap sm(rows, perRow, std::make_pair(1, bizRows));

    for (const auto& b : bks) {
        if (b.flightNumber == fn) {
            std::string code = b.seatNumber;
            int row = 0; char c = 'A';
            for (char ch : code) {
                if (std::isdigit(ch)) row = row * 10 + (ch - '0');
                else c = ch;
            }
            sm.markSeatOccupied(row, c - 'A');
        }
    }

    sm.display();

    std::string seat;
    std::cout << "Введите место (например, 12C): "; std::cin >> seat;

    int row = 0; char c = 'A';
    for (char ch : seat) {
        if (std::isdigit(ch)) row = row * 10 + (ch - '0');
        else c = ch;
    }
    int seatIdx = c - 'A';
    if (row < 1 || row > rows || seatIdx < 0 || seatIdx >= perRow || !sm.isSeatAvailable(row, seatIdx)) {
        std::cout << "Место занято или некорректно!\n"; pause(); return;
    }

    double price = sm.getSeatPrice(row, flIt->basePriceEconomy, flIt->basePriceBusiness);
    std::cout << "Стоимость: " << price << " руб.\n";

    std::string pp;
    std::cout << "Паспорт пассажира: "; std::cin >> pp;
    auto psIt = std::find_if(ps.begin(), ps.end(), [&](const Passenger& p) { return p.passportNumber == pp; });
    Passenger* targetPs = nullptr;

    if (psIt == ps.end()) {
        std::string fn, ln, dob, phone, email;
        std::cout << "Новый пассажир. Имя: "; std::cin >> fn;
        std::cout << "Фамилия: "; std::cin >> ln;
        std::cout << "Дата рождения (YYYY-MM-DD): "; std::cin >> dob;
        while (!isValidDate(dob)) { std::cout << "Неверно! "; std::cin >> dob; }
        std::cout << "Телефон (+7...): "; std::cin >> phone;
        while (!isValidPhone(phone)) { std::cout << "Неверно! "; std::cin >> phone; }
        std::cout << "Email: "; std::cin >> email;
        while (!isValidEmail(email)) { std::cout << "Неверно! "; std::cin >> email; }
        std::string id = "P" + std::to_string(ps.size() + 1);
        ps.emplace_back(id, fn, ln, dob, pp, phone, email);
        targetPs = &ps.back();
        Passenger::saveAll(ps);
    }
    else {
        targetPs = &(*psIt);
    }

    Booking bk(generatePNR(), fn, targetPs->passengerId, seat, BookingStatus::Confirmed, price, "2025-11-26");
    bks.push_back(bk);
    Booking::saveAll(bks);
    std::cout << "\nБронирование успешно создано! PNR: " << bk.bookingNumber << "\n";
    pause();
}

void moduleEmployees(std::vector<Employee>& emps) {
    int choice;
    while (true) {
        clearScreen();
        displayTableEmployees(emps);
        std::cout << "\n1. Добавить сотрудника\n2. Удалить сотрудника\n3. Назад\n";
        std::cin >> choice;

        if (choice == 1) {
            std::string id, fn, ln, login, pwd;
            int roleInt;
            std::cout << "ID: "; std::cin >> id;
            std::cout << "Имя: "; std::cin >> fn;
            std::cout << "Фамилия: "; std::cin >> ln;
            std::cout << "Роль (1-Admin, 2-Dispatcher, 3-FlightAttendant): "; std::cin >> roleInt;
            EmployeeRole role = (roleInt == 1) ? EmployeeRole::Admin :
                (roleInt == 2) ? EmployeeRole::Dispatcher : EmployeeRole::FlightAttendant;
            std::cout << "Логин: "; std::cin >> login;
            std::cout << "Пароль: "; std::cin >> pwd;
            emps.emplace_back(id, fn, ln, role, login, pwd);
            Employee::saveAll(emps);
        }
        else if (choice == 2) {
            std::string id;
            std::cout << "ID сотрудника для удаления: "; std::cin >> id;
            emps.erase(std::remove_if(emps.begin(), emps.end(), [&](const Employee& e) { return e.employeeId == id; }), emps.end());
            Employee::saveAll(emps);
        }
        else if (choice == 3) break;
        else { std::cout << "Неверный выбор.\n"; pause(); }
    }
}

void moduleAirports(std::vector<Airport>& aps) {
    int choice;
    while (true) {
        clearScreen();
        displayTableAirports(aps);
        std::cout << "\n1. Добавить аэропорт\n2. Удалить аэропорт\n3. Назад\n";
        std::cin >> choice;

        if (choice == 1) {
            std::string code, name, city, country;
            std::cout << "Код (например, SVO): "; std::cin >> code;
            std::cout << "Название: "; std::cin >> name;
            std::cout << "Город: "; std::cin >> city;
            std::cout << "Страна: "; std::cin >> country;
            aps.emplace_back(code, name, city, country);
            Airport::saveAll(aps);
        }
        else if (choice == 2) {
            std::string code;
            std::cout << "Код аэропорта для удаления: "; std::cin >> code;
            aps.erase(std::remove_if(aps.begin(), aps.end(), [&](const Airport& a) { return a.code == code; }), aps.end());
            Airport::saveAll(aps);
        }
        else if (choice == 3) break;
        else { std::cout << "Неверный выбор.\n"; pause(); }
    }
}

// ---------- МЕНЮ ПО РОЛЯМ ----------
void menuAdmin(
    std::vector<Aircraft>& acs,
    std::vector<Flight>& fls,
    std::vector<Passenger>& ps,
    std::vector<Booking>& bks,
    std::vector<Employee>& emps,
    std::vector<Airport>& aps
) {
    int choice;
    while (true) {
        clearScreen();
        std::cout << "=== Меню Администратора ===\n";
        std::cout << "1. Воздушные суда\n2. Рейсы\n3. Пассажиры\n4. Бронирования\n";
        std::cout << "5. Сотрудники\n6. Аэропорты\n7. Выход\n";
        std::cin >> choice;
        switch (choice) {
        case 1: moduleAircraft(acs); break;
        case 2: moduleFlights(fls, acs); break;
        case 3: modulePassengers(ps); break;
        case 4: moduleBookings(fls, ps, bks); break;
        case 5: moduleEmployees(emps); break;
        case 6: moduleAirports(aps); break;
        case 7: return;
        default: std::cout << "Неверный выбор.\n"; pause();
        }
    }
}

void menuDispatcher(
    std::vector<Aircraft>& acs,
    std::vector<Flight>& fls,
    std::vector<Passenger>& ps,
    std::vector<Booking>& bks,
    std::vector<Airport>& aps
) {
    int choice;
    while (true) {
        clearScreen();
        std::cout << "=== Меню Диспетчера ===\n";
        std::cout << "1. Воздушные суда (просмотр)\n2. Рейсы\n3. Пассажиры\n4. Бронирования\n5. Аэропорты (просмотр)\n6. Выход\n";
        std::cin >> choice;
        switch (choice) {
        case 1: displayTableAircraft(acs); pause(); break;
        case 2: moduleFlights(fls, acs); break;
        case 3: modulePassengers(ps); break;
        case 4: moduleBookings(fls, ps, bks); break;
        case 5: displayTableAirports(aps); pause(); break;
        case 6: return;
        default: std::cout << "Неверный выбор.\n"; pause();
        }
    }
}

void menuFlightAttendant(
    std::vector<Flight>& fls,
    std::vector<Booking>& bks
) {
    int choice;
    while (true) {
        clearScreen();
        std::cout << "=== Меню Бортпроводника ===\n";
        std::cout << "1. Просмотр рейсов\n2. Схемы салонов\n3. Изменить статус рейса\n4. Выход\n";
        std::cin >> choice;
        if (choice == 1) {
            displayTableFlights(fls); pause();
        }
        else if (choice == 2) {
            if (fls.empty()) { std::cout << "Нет рейсов.\n"; pause(); continue; }
            int rows = 30, perRow = 6;
            SeatMap sm(rows, perRow, std::make_pair(1, 4));
            for (const auto& b : bks) {
                std::string code = b.seatNumber;
                int row = 0; char c = 'A';
                for (char ch : code) {
                    if (std::isdigit(ch)) row = row * 10 + (ch - '0');
                    else c = ch;
                }
                sm.markSeatOccupied(row, c - 'A');
            }
            sm.display();
            pause();
        }
        else if (choice == 3) {
            displayTableFlights(fls);
            std::string fn;
            std::cout << "Введите номер рейса: "; std::cin >> fn;
            auto it = std::find_if(fls.begin(), fls.end(), [&](const Flight& f) { return f.flightNumber == fn; });
            if (it != fls.end()) {
                std::cout << "Текущий статус: " << Flight::statusToString(it->status) << "\n";
                std::cout << "Новый статус (Scheduled/Departed/Delayed/Arrived/Canceled): ";
                std::string newStatusStr;
                std::cin >> newStatusStr;
                it->status = Flight::stringToStatus(newStatusStr);
                Flight::saveAll(fls);
                std::cout << "Статус обновлён.\n";
            }
            else {
                std::cout << "Рейс не найден!\n";
            }
            pause();
        }
        else if (choice == 4) {
            return;
        }
        else {
            std::cout << "Неверный выбор.\n"; pause();
        }
    }
}

// ---------- MAIN ----------
int main() {
    ensureDataDir();

    std::vector<Aircraft> acs;
    std::vector<Flight> fls;
    std::vector<Passenger> ps;
    std::vector<Booking> bks;
    std::vector<Employee> emps;
    std::vector<Airport> aps;

    Aircraft::loadAll(acs);
    Flight::loadAll(fls);
    Passenger::loadAll(ps);
    Booking::loadAll(bks);
    Employee::loadAll(emps);
    Airport::loadAll(aps);

    // Инициализация трёх пользователей по ТЗ
    if (emps.empty()) {
        emps.emplace_back("E001", "Admin", "Adminov", EmployeeRole::Admin, "admin", "123");
        emps.emplace_back("E002", "Dispatcher", "Ivanov", EmployeeRole::Dispatcher, "disp", "123");
        emps.emplace_back("E003", "Attendant", "Petrova", EmployeeRole::FlightAttendant, "att", "123");
        Employee::saveAll(emps);
    }
    if (acs.empty()) {
        acs.emplace_back("RA123", "Boeing 737", 180, 24, 156);
        Aircraft::saveAll(acs);
    }
    if (fls.empty()) {
        fls.emplace_back("PF100", "RA123", "SVO", "LED", "2025-11-30 10:00", "2025-11-30 12:00", 5000, 15000);
        Flight::saveAll(fls);
    }
    if (aps.empty()) {
        aps.emplace_back("SVO", "Sheremetyevo", "Moscow", "Russia");
        aps.emplace_back("LED", "Pulkovo", "Saint Petersburg", "Russia");
        Airport::saveAll(aps);
    }

    // Аутентификация
    std::string login, pwd;
    std::cout << "=== Pinky Fly ===\nЛогин: ";
    std::cin >> login;
    std::cout << "Пароль: ";
    std::cin >> pwd;

    Employee* user = nullptr;
    for (auto& e : emps) {
        if (e.login == login && e.authenticate(pwd)) {
            user = &e;
            break;
        }
    }

    if (!user) {
        std::cout << "Ошибка входа!\n";
        pause();
        return 1;
    }

    // Меню по роли
    switch (user->role) {
    case EmployeeRole::Admin:
        menuAdmin(acs, fls, ps, bks, emps, aps);

        break;
    case EmployeeRole::Dispatcher:
        menuDispatcher(acs, fls, ps, bks, aps);
        break;
    case EmployeeRole::FlightAttendant:
        menuFlightAttendant(fls, bks);
        break;
    }

    // Сохранение при выходе
    Aircraft::saveAll(acs);
    Flight::saveAll(fls);
    Passenger::saveAll(ps);
    Booking::saveAll(bks);
    Employee::saveAll(emps);
    Airport::saveAll(aps);

    return 0;
}
