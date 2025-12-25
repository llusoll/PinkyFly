#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cctype>
#include <string>

// Заголовки проекта
#include "Aircraft.h"
#include "Flight.h"
#include "Passenger.h"
#include "Booking.h"
#include "Employee.h"
#include "Airport.h"
#include "SeatMap.h"
#include "Utils.h"
#include "BookingRegistry.h"
#include "BusinessBooking.h"
#include "EconomyBooking.h"

#include "CompileTimeUtils.h"
#include "SeatConfig.h"

// Глобальные данные проекта
std::vector<std::unique_ptr<Aircraft>> acs;
std::vector<std::unique_ptr<Flight>> fls;
std::vector<std::unique_ptr<Passenger>> ps;
std::vector<std::unique_ptr<Booking>> bks;
std::vector<std::unique_ptr<Employee>> emps;
std::vector<std::unique_ptr<Airport>> aps;

// === ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    std::cout << "\nНажмите Enter, чтобы продолжить...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string generatePNR() {
    static int counter = 1000;
    return "PNR" + std::to_string(++counter);
}

// === ТАБЛИЧНЫЕ ВЫВОДЫ ===

void displayTableAircraft(const std::vector<std::unique_ptr<Aircraft>>& list) {
    if (list.empty()) {
        std::cout << "Нет данных.\n";
        return;
    }
    std::cout << "\n=== Воздушные суда ===\n";
    std::cout << std::left
        << std::setw(20) << "Борт"
        << std::setw(20) << "Модель"
        << std::setw(20) << "Всего"
        << std::setw(20) << "Бизнес"
        << std::setw(20) << "Статус"
        << std::setw(20) << "Налёт\n";
    for (const auto& a : list) {
        std::cout << std::left
            << std::setw(20) << a->tailNumber
            << std::setw(20) << a->model
            << std::setw(20) << a->totalSeats
            << std::setw(20) << a->businessSeats
            << std::setw(20) << Aircraft::statusToString(a->status)
            << std::setw(20) << a->flightHours << "\n";
    }
}

void displayTableFlights(const std::vector<std::unique_ptr<Flight>>& list) {
    if (list.empty()) {
        std::cout << "Нет данных.\n";
        return;
    }
    std::cout << "\n=== Рейсы ===\n";
    std::cout << std::left
        << std::setw(20) << "Рейс"
        << std::setw(25) << "Маршрут"
        << std::setw(20) << "Вылет (расп.)"
        << std::setw(20) << "Статус\n";
    for (const auto& f : list) {
        std::cout << std::left
            << std::setw(20) << f->flightNumber
            << std::setw(25) << (f->departureAirport + " → " + f->arrivalAirport)
            << std::setw(20) << f->scheduledDeparture
            << std::setw(20) << Flight::statusToString(f->status) << "\n";
    }
}

void displayTablePassengers(const std::vector<std::unique_ptr<Passenger>>& list) {
    if (list.empty()) {
        std::cout << "Нет данных.\n";
        return;
    }
    std::cout << "\n=== Пассажиры ===\n";
    std::cout << std::left
        << std::setw(20) << "Паспорт"
        << std::setw(25) << "ФИО"
        << std::setw(20) << "Телефон"
        << std::setw(25) << "Email\n";
    for (const auto& p : list) {
        std::cout << std::left
            << std::setw(20) << p->passportNumber
            << std::setw(25) << (p->lastName + " " + p->firstName)
            << std::setw(20) << p->contactPhone
            << std::setw(25) << p->email << "\n";
    }
}

void displayTableBookings(const std::vector<std::unique_ptr<Booking>>& list) {
    if (list.empty()) {
        std::cout << "Нет данных.\n";
        return;
    }
    std::cout << "\n=== Бронирования ===\n";
    std::cout << std::left
        << std::setw(20) << "PNR"
        << std::setw(20) << "Рейс"
        << std::setw(25) << "Пассажир (ID)"

        << std::setw(15) << "Место"
        << std::setw(20) << "Статус\n";
    for (const auto& b : list) {
        std::cout << std::left
            << std::setw(20) << b->getBookingNumber()
            << std::setw(20) << b->getFlightNumber()
            << std::setw(25) << b->getPassengerId()
            << std::setw(15) << b->getSeatNumber()
            << std::setw(20) << Booking::statusToString(b->getStatus()) << "\n";
    }
}

void displayTableEmployees(const std::vector<std::unique_ptr<Employee>>& list) {
    if (list.empty()) {
        std::cout << "Нет данных.\n";
        return;
    }
    std::cout << "\n=== Сотрудники ===\n";
    std::cout << std::left
        << std::setw(15) << "ID"
        << std::setw(25) << "ФИО"
        << std::setw(20) << "Роль"
        << std::setw(20) << "Логин\n";
    for (const auto& e : list) {
        std::cout << std::left
            << std::setw(15) << e->employeeId
            << std::setw(25) << (e->lastName + " " + e->firstName)
            << std::setw(20) << Employee::roleToString(e->role)
            << std::setw(20) << e->login << "\n";
    }
}

void displayTableAirports(const std::vector<std::unique_ptr<Airport>>& list) {
    if (list.empty()) {
        std::cout << "Нет данных.\n";
        return;
    }
    std::cout << "\n=== Аэропорты ===\n";
    std::cout << std::left
        << std::setw(10) << "Код"
        << std::setw(25) << "Название"
        << std::setw(20) << "Город"
        << std::setw(20) << "Страна\n";
    for (const auto& a : list) {
        std::cout << std::left
            << std::setw(10) << a->code
            << std::setw(25) << a->name
            << std::setw(20) << a->city
            << std::setw(20) << a->country << "\n";
    }
}

// === МОДУЛИ ===

void moduleAircraft(std::vector<std::unique_ptr<Aircraft>>& acs) {
    int choice;
    while (true) {
        clearScreen();
        displayTableAircraft(acs);
        std::cout << "\n1. Добавить ВС\n2. Редактировать ВС\n3. Удалить ВС\n4. Назад\n";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        try {
            if (choice == 1) {
                std::string tn, mdl;
                int t, b, e;
                std::cout << "Бортовой номер: "; std::cin >> tn;
                std::cout << "Модель: "; std::cin >> mdl;
                std::cout << "Всего мест: "; std::cin >> t;
                std::cout << "Мест бизнес-класса: "; std::cin >> b;
                std::cout << "Мест эконом-класса: "; std::cin >> e;
                acs.push_back(std::make_unique<Aircraft>(tn, mdl, t, b, e));
                Aircraft::saveAll(acs);
            }
            else if (choice == 2) {
                std::string tn;
                std::cout << "Бортовой номер для редактирования: "; std::cin >> tn;
                auto it = std::find_if(acs.begin(), acs.end(),
                    [&](const auto& a) { return a->tailNumber == tn; });
                if (it != acs.end()) {
                    std::cout << "Новая модель: "; std::cin >> (*it)->model;
                    std::cout << "Всего мест: "; std::cin >> (*it)->totalSeats;
                    std::cout << "Бизнес: "; std::cin >> (*it)->businessSeats;
                    std::cout << "Эконом: "; std::cin >> (*it)->economySeats;
                    std::string st;
                    std::cout << "Статус (InService/Maintenance/Retired): "; std::cin >> st;
                    (*it)->status = Aircraft::stringToStatus(st);
                    std::cout << "Дата ТО (ГГГГ-ММ-ДД): "; std::cin >> (*it)->maintenanceSchedule;
                    Aircraft::saveAll(acs);
                }
                else {
                    std::cout << "ВС не найдено!\n";
                    pause();
                }
            }
            else if (choice == 3) {
                std::string tn;

                std::cout << "Бортовой номер для удаления: "; std::cin >> tn;
                acs.erase(std::remove_if(acs.begin(), acs.end(),
                    [&](const auto& a) { return a->tailNumber == tn; }), acs.end());
                Aircraft::saveAll(acs);
            }
            else if (choice == 4) {
                break;
            }
            else {
                std::cout << "Неверный выбор.\n";
                pause();
            }
        }
        catch (const std::exception& ex) {
            std::cout << "Ошибка: " << ex.what() << "\n";
            pause();
        }
    }
}

void moduleFlights(std::vector<std::unique_ptr<Flight>>& fls, const std::vector<std::unique_ptr<Aircraft>>& acs) {
    int choice;
    while (true) {
        clearScreen();
        displayTableFlights(fls);
        std::cout << "\n1. Добавить рейс\n2. Изменить статус\n3. Задержать рейс\n4. Удалить рейс\n5. Назад\n";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        try {
            if (choice == 1) {
                std::string fn, tail, dep, arr, schedDep, schedArr;
                double eco, biz;
                std::cout << "Номер рейса: "; std::cin >> fn;
                std::cout << "Бортовой номер ВС: "; std::cin >> tail;
                if (std::find_if(acs.begin(), acs.end(),
                    [&](const auto& a) { return a->tailNumber == tail; }) == acs.end()) {
                    std::cout << "ВС не найден!\n";
                    pause();
                    continue;
                }
                std::cout << "Аэропорт вылета (код): "; std::cin >> dep;
                std::cout << "Аэропорт прилёта (код): "; std::cin >> arr;
                std::cout << "Время вылета (ГГГГ-ММ-ДД ЧЧ:ММ): "; std::cin >> schedDep;
                std::cout << "Время прилёта (ГГГГ-ММ-ДД ЧЧ:ММ): "; std::cin >> schedArr;
                std::cout << "Базовая цена эконом: "; std::cin >> eco;
                std::cout << "Базовая цена бизнес: "; std::cin >> biz;
                fls.push_back(std::make_unique<Flight>(fn, tail, dep, arr, schedDep, schedArr, eco, biz));
                Flight::saveAll(fls);
            }
            else if (choice == 2) {
                std::string fn;
                std::cout << "Номер рейса: "; std::cin >> fn;
                auto it = std::find_if(fls.begin(), fls.end(),
                    [&](const auto& f) { return f->flightNumber == fn; });
                if (it != fls.end()) {
                    std::string st;
                    std::cout << "Новый статус (Scheduled/Departed/Delayed/Arrived/Canceled): "; std::cin >> st;
                    (*it)->status = Flight::stringToStatus(st);
                    Flight::saveAll(fls);
                }
                else {
                    std::cout << "Рейс не найден!\n";
                    pause();
                }
            }
            else if (choice == 3) {
                std::string fn, newTime;
                std::cout << "Номер рейса: "; std::cin >> fn;
                std::cout << "Новое время вылета (ГГГГ-ММ-ДД ЧЧ:ММ): "; std::cin >> newTime;
                auto it = std::find_if(fls.begin(), fls.end(),
                    [&](const auto& f) { return f->flightNumber == fn; });
                if (it != fls.end()) {
                    (*it)->delayFlight(newTime);
                    Flight::saveAll(fls);
                }
                else {
                    std::cout << "Рейс не найден!\n";
                    pause();
                }
            }
            else if (choice == 4) {
                std::string fn;
                std::cout << "Номер рейса для удаления: "; std::cin >> fn;
                fls.erase(std::remove_if(fls.begin(), fls.end(),
                    [&](const auto& f) { return f->flightNumber == fn; }), fls.end());
                Flight::saveAll(fls);
            }
            else if (choice == 5) {
                break;
            }
            else {
                std::cout << "Неверный выбор.\n";
                pause();
            }

        }
        catch (const std::exception& ex) {
            std::cout << "Ошибка: " << ex.what() << "\n";
            pause();
        }
    }
}

void modulePassengers(std::vector<std::unique_ptr<Passenger>>& ps) {
    int choice;
    while (true) {
        clearScreen();
        displayTablePassengers(ps);
        std::cout << "\n1. Добавить пассажира\n2. Редактировать контактные данные\n3. Удалить пассажира\n4. Назад\n";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        try {
            if (choice == 1) {
                std::string fn, ln, dob, pp, phone, email;
                std::cout << "Имя: "; std::cin >> fn;
                std::cout << "Фамилия: "; std::cin >> ln;
                std::cout << "Дата рождения (ГГГГ-ММ-ДД): "; std::cin >> dob;
                std::cout << "Паспорт (6–15 цифр): "; std::cin >> pp;
                std::cout << "Телефон (+7... или 8...): "; std::cin >> phone;
                std::cout << "Email: "; std::cin >> email;
                std::string id = "P" + std::to_string(ps.size() + 1);
                ps.push_back(std::make_unique<Passenger>(id, fn, ln, dob, pp, phone, email));
                Passenger::saveAll(ps);
            }
            else if (choice == 2) {
                std::string pp;
                std::cout << "Паспорт для редактирования: "; std::cin >> pp;
                auto it = std::find_if(ps.begin(), ps.end(),
                    [&](const auto& p) { return p->passportNumber == pp; });
                if (it != ps.end()) {
                    std::string phone, email;
                    std::cout << "Новый телефон: "; std::cin >> phone;
                    std::cout << "Новый email: "; std::cin >> email;
                    (*it)->updateContactInfo(phone, email);
                    Passenger::saveAll(ps);
                }
                else {
                    std::cout << "Пассажир не найден!\n";
                    pause();
                }
            }
            else if (choice == 3) {
                std::string pp;
                std::cout << "Паспорт для удаления: "; std::cin >> pp;
                ps.erase(std::remove_if(ps.begin(), ps.end(),
                    [&](const auto& p) { return p->passportNumber == pp; }), ps.end());
                Passenger::saveAll(ps);
            }
            else if (choice == 4) {
                break;
            }
            else {
                std::cout << "Неверный выбор.\n";
                pause();
            }
        }
        catch (const std::exception& ex) {
            std::cout << "Ошибка: " << ex.what() << "\n";
            pause();
        }
    }
}

void moduleBookings(std::vector<std::unique_ptr<Flight>>& fls,
    std::vector<std::unique_ptr<Passenger>>& ps,
    std::vector<std::unique_ptr<Booking>>& bks) {
    clearScreen();
    displayTableBookings(bks); // ← ДОБАВЛЕНО: показываем все бронирования
    std::cout << "\nХотите создать новое бронирование? (y/n): ";
    char ch;
    std::cin >> ch;
    if (ch != 'y' && ch != 'Y') {
        return; // выход, если "нет"
    }
    if (fls.empty()) {
        std::cout << "Нет рейсов для бронирования!\n";
        pause();
        return;
    }
    displayTableFlights(fls);
    std::string fn;
    std::cout << "Введите номер рейса: "; std::cin >> fn;
    auto flIt = std::find_if(fls.begin(), fls.end(),
        [&](const auto& f) { return f->flightNumber == fn; });
    if (flIt == fls.end()) {
        std::cout << "Рейс не найден!\n";
        pause();
        return;
    }

    try {
        int rows = 30, perRow = 6;
        int bizRows = ((*flIt)->aircraftTail == "RA123") ? 4 : 2;
        SeatMap sm(rows, perRow, std::make_pair(1, bizRows));

        for (const auto& b : bks) {
            if (b->getFlightNumber() == fn) {
                std::string code = b->getSeatNumber();
                int row = 0;
                char c = 'A';
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
        int row = 0;
        char c = 'A';
        for (char ch : seat) {
            if (std::isdigit(ch)) row = row * 10 + (ch - '0');
            else c = ch;
        }
        int seatIdx = c - 'A';

        if (row < 1 || seatIdx < 0 || !sm.isSeatAvailable(row, seatIdx)) {
            std::cout << "Место занято или некорректно!\n";
            pause();
            return;
        }

        double price = sm.getSeatPrice(row, (*flIt)->basePriceEconomy, (*flIt)->basePriceBusiness);
        std::cout << "Стоимость: " << price << " руб.\n";

        std::string pp;
        std::cout << "Паспорт пассажира: "; std::cin >> pp;
        Passenger* targetPs = nullptr;
        auto psIt = std::find_if(ps.begin(), ps.end(),
            [&](const auto& p) { return p->passportNumber == pp; });
        if (psIt == ps.end()) {
            std::string fn, ln, dob, phone, email;
            std::cout << "Новый пассажир.\n";
            std::cout << "Имя: "; std::cin >> fn;
            std::cout << "Фамилия: "; std::cin >> ln;
            std::cout << "Дата рождения (ГГГГ-ММ-ДД): "; std::cin >> dob;
            std::cout << "Телефон (+7...): "; std::cin >> phone;
            std::cout << "Email: "; std::cin >> email;
            std::string id = "P" + std::to_string(ps.size() + 1);
            ps.push_back(std::make_unique<Passenger>(id, fn, ln, dob, pp, phone, email));
            targetPs = ps.back().get();
            Passenger::saveAll(ps);
        }
        else {
            targetPs = psIt->get();
        }

        std::unique_ptr<Booking> newBooking;
        if (row <= bizRows) {
            newBooking = std::make_unique<BusinessBooking>(
                generatePNR(), fn, targetPs->passengerId, seat,
                BookingStatus::Confirmed, (*flIt)->basePriceBusiness, "2025-12-20"
            );
        }
        else {
            newBooking = std::make_unique<EconomyBooking>(
                generatePNR(), fn, targetPs->passengerId, seat,
                BookingStatus::Confirmed, (*flIt)->basePriceEconomy, "2025-12-20"
            );
        }
        bks.push_back(std::move(newBooking));
        Booking::saveAll(bks);
        std::cout << "\nБронирование успешно создано! PNR: " << bks.back()->getBookingNumber() << "\n";
        pause();

    }
    catch (const std::exception& ex) {
        std::cout << "Ошибка при бронировании: " << ex.what() << "\n";
        pause();
    }
}

void moduleEmployees(std::vector<std::unique_ptr<Employee>>& emps) {
    int choice;
    while (true) {
        clearScreen();
        displayTableEmployees(emps);
        std::cout << "\n1. Добавить сотрудника\n2. Удалить сотрудника\n3. Назад\n";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        try {
            if (choice == 1) {
                std::string id, fn, ln, login, pwd;
                int roleInt;
                std::cout << "ID: "; std::cin >> id;
                std::cout << "Имя: "; std::cin >> fn;
                std::cout << "Фамилия: "; std::cin >> ln;
                std::cout << "Роль (1–Admin, 2–Dispatcher, 3–FlightAttendant): "; std::cin >> roleInt;
                EmployeeRole role = (roleInt == 1) ? EmployeeRole::Admin
                    : (roleInt == 2) ? EmployeeRole::Dispatcher
                    : EmployeeRole::FlightAttendant;
                std::cout << "Логин: "; std::cin >> login;
                std::cout << "Пароль: "; std::cin >> pwd;
                emps.push_back(std::make_unique<Employee>(id, fn, ln, role, login, pwd));
                Employee::saveAll(emps);
            }
            else if (choice == 2) {
                std::string id;
                std::cout << "ID сотрудника для удаления: "; std::cin >> id;
                emps.erase(std::remove_if(emps.begin(), emps.end(),
                    [&](const auto& e) { return e->employeeId == id; }), emps.end());

                Employee::saveAll(emps);
            }
            else if (choice == 3) {
                break;
            }
            else {
                std::cout << "Неверный выбор.\n";
                pause();
            }
        }
        catch (const std::exception& ex) {
            std::cout << "Ошибка: " << ex.what() << "\n";
            pause();
        }
    }
}

void moduleAirports(std::vector<std::unique_ptr<Airport>>& aps) {
    int choice;
    while (true) {
        clearScreen();
        displayTableAirports(aps);
        std::cout << "\n1. Добавить аэропорт\n2. Удалить аэропорт\n3. Назад\n";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        try {
            if (choice == 1) {
                std::string code, name, city, country;
                std::cout << "Код (например, SVO): "; std::cin >> code;
                std::cout << "Название: "; std::cin >> name;
                std::cout << "Город: "; std::cin >> city;
                std::cout << "Страна: "; std::cin >> country;
                aps.push_back(std::make_unique<Airport>(code, name, city, country));
                Airport::saveAll(aps);
            }
            else if (choice == 2) {
                std::string code;
                std::cout << "Код аэропорта для удаления: "; std::cin >> code;
                aps.erase(std::remove_if(aps.begin(), aps.end(),
                    [&](const auto& a) { return a->code == code; }), aps.end());
                Airport::saveAll(aps);
            }
            else if (choice == 3) {
                break;
            }
            else {
                std::cout << "Неверный выбор.\n";
                pause();
            }
        }
        catch (const std::exception& ex) {
            std::cout << "Ошибка: " << ex.what() << "\n";
            pause();
        }
    }
}

// === МЕНЮ ПО РОЛЯМ ===

void menuAdmin(
    std::vector<std::unique_ptr<Aircraft>>& acs,
    std::vector<std::unique_ptr<Flight>>& fls,
    std::vector<std::unique_ptr<Passenger>>& ps,
    std::vector<std::unique_ptr<Booking>>& bks,
    std::vector<std::unique_ptr<Employee>>& emps,
    std::vector<std::unique_ptr<Airport>>& aps) {

    int choice;
    while (true) {
        clearScreen();
        std::cout << "=== Меню Администратора ===\n";
        std::cout << "1. Воздушные суда\n";
        std::cout << "2. Рейсы\n";
        std::cout << "3. Пассажиры\n";
        std::cout << "4. Бронирования\n";
        std::cout << "5. Сотрудники\n";
        std::cout << "6. Аэропорты\n";
        std::cout << "7. Аналитика\n";
        std::cout << "8. Выход\n";
        std::cout << "Выберите пункт: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice) {
        case 1: moduleAircraft(acs); break;
        case 2: moduleFlights(fls, acs); break;
        case 3: modulePassengers(ps); break;
        case 4: moduleBookings(fls, ps, bks); break;
        case 5: moduleEmployees(emps); break;
        case 6: moduleAirports(aps); break;
        case 7: {
            clearScreen();
            

            std::vector<double> prices;
            for (const auto& b : bks) prices.push_back(b->calculatePrice());
            double avg = safe_average(prices);
            std::cout << "\nСредняя цена всех бронирований: " << avg << " руб.\n";

            BookingRegistry<EconomyBooking> ecoReg;
            BookingRegistry<BusinessBooking> bizReg;
            for (auto& b : bks) {
                if (dynamic_cast<EconomyBooking*>(b.get())) {
                    ecoReg.add(std::unique_ptr<EconomyBooking>(
                        static_cast<EconomyBooking*>(b.release())

                    ));
                }
                else if (dynamic_cast<BusinessBooking*>(b.get())) {
                    bizReg.add(std::unique_ptr<BusinessBooking>(
                        static_cast<BusinessBooking*>(b.release())
                    ));
                }
            }

            ecoReg.sortByPrice();
            std::cout << "\nЭконом-бронирования (по возрастанию цены):\n";
            for (const auto* b : ecoReg.getAll()) {
                std::cout << "  " << b->getBookingNumber() << " — " << b->calculatePrice() << " руб.\n";
            }

            auto found = bizReg.findByPNR("PNR1001");
            if (found) {
                std::cout << "\nНайдено бизнес-бронирование: " << found->getDescription()
                    << ", цена: " << found->calculatePrice() << "\n";
            }

            std::cout << "\nВыручка от эконома: " << ecoReg.getTotalRevenue() << " руб.\n";
            std::cout << "Выручка от бизнеса: " << bizReg.getTotalRevenue() << " руб.\n";

            // Возврат объектов в основной список через клонирование
            for (const auto* b : ecoReg.getAll()) bks.push_back(b->clone());
            for (const auto* b : bizReg.getAll()) bks.push_back(b->clone());

            pause();
            break;
        }
        
        case 8: return;
        default: std::cout << "Неверный выбор.\n"; pause();
        }
    }
}

void menuDispatcher(
    std::vector<std::unique_ptr<Aircraft>>& acs,
    std::vector<std::unique_ptr<Flight>>& fls,
    std::vector<std::unique_ptr<Passenger>>& ps,
    std::vector<std::unique_ptr<Booking>>& bks,
    std::vector<std::unique_ptr<Airport>>& aps) {

    int choice;
    while (true) {
        clearScreen();
        std::cout << "=== Меню Диспетчера ===\n";
        std::cout << "1. Воздушные суда (просмотр)\n";
        std::cout << "2. Рейсы\n";
        std::cout << "3. Пассажиры\n";
        std::cout << "4. Бронирования\n";
        std::cout << "5. Аэропорты (просмотр)\n";
        std::cout << "6. Выход\n";
        std::cout << "Выберите пункт: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

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
    std::vector<std::unique_ptr<Flight>>& fls,
    std::vector<std::unique_ptr<Booking>>& bks) {

    int choice;
    while (true) {
        clearScreen();
        std::cout << "=== Меню Бортпроводника ===\n";
        std::cout << "1. Просмотр рейсов\n";
        std::cout << "2. Схемы салонов\n";
        std::cout << "3. Изменить статус рейса\n";
        std::cout << "4. Выход\n";
        std::cout << "Выберите пункт: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        if (choice == 1) {
            displayTableFlights(fls); pause();
        }
        else if (choice == 2) {
            if (fls.empty()) {
                std::cout << "Нет рейсов.\n";
                pause();
                continue;
            }
            SeatMap sm(30, 6, std::make_pair(1, 4));
            for (const auto& b : bks) {
                std::string code = b->getSeatNumber();
                int row = 0;
                char c = 'A';
                for (char ch : code) {
                    if (std::isdigit(ch)) row = row * 10 + (ch - '0');
                    else c = ch;
                }
                if (row >= 1 && row <= 30) {
                    sm.markSeatOccupied(row, c - 'A');
                }
            }
            sm.display();
            pause();
        }
        else if (choice == 3) {
            displayTableFlights(fls);
            std::string fn;
            std::cout << "Введите номер рейса: "; std::cin >> fn;
            auto it = std::find_if(fls.begin(), fls.end(),
                [&](const auto& f) { return f->flightNumber == fn; });
            if (it != fls.end()) {
                std::cout << "Текущий статус: " << Flight::statusToString((*it)->status) << "\n";
                std::string newStatusStr;
                std::cout << "Новый статус (Scheduled/Departed/Delayed/Arrived/Canceled): ";
                std::cin >> newStatusStr;
                (*it)->updateStatus(Flight::stringToStatus(newStatusStr));
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
            std::cout << "Неверный выбор.\n";
            pause();
        }
    }
}

// === MAIN ===

int main() {
    try {
        ensureDataDir();

        Aircraft::loadAll(acs);
        Flight::loadAll(fls);
        Passenger::loadAll(ps);
        Booking::loadAll(bks);
        Employee::loadAll(emps);
        Airport::loadAll(aps);

        // Инициализация по умолчанию (если данные отсутствуют)
        if (emps.empty()) {
            emps.push_back(std::make_unique<Employee>("E001", "Admin", "Adminov", EmployeeRole::Admin, "admin", "123"));
            emps.push_back(std::make_unique<Employee>("E002", "Dispatcher", "Ivanov", EmployeeRole::Dispatcher, "disp", "123"));
            emps.push_back(std::make_unique<Employee>("E003", "Attendant", "Petrova", EmployeeRole::FlightAttendant, "att", "123"));
            Employee::saveAll(emps);
        }
        if (acs.empty()) {
            acs.push_back(std::make_unique<Aircraft>("RA123", "Boeing 737", 180, 24, 156));
            Aircraft::saveAll(acs);
        }
        if (fls.empty()) {
            fls.push_back(std::make_unique<Flight>("PF100", "RA123", "SVO", "LED", "2025-12-15 10:00", "2025-12-15 12:00", 5000, 15000));
            Flight::saveAll(fls);
        }
        if (aps.empty()) {
            aps.push_back(std::make_unique<Airport>("SVO", "Шереметьево", "Москва", "Россия"));
            aps.push_back(std::make_unique<Airport>("LED", "Пулково", "Санкт-Петербург", "Россия"));
            Airport::saveAll(aps);
        }

        // Аутентификация
        std::string login, pwd;
        std::cout << "=== Pinky Fly ===\nЛогин: "; std::cin >> login;
        std::cout << "Пароль: "; std::cin >> pwd;

        Employee* user = nullptr;
        for (const auto& e : emps) {
            if (e->login == login && e->authenticate(pwd)) {
                user = e.get();
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

    }
    catch (const std::exception& ex) {
        std::cerr << "Критическая ошибка: " << ex.what() << "\n";
        pause();
        return 1;
    }
    return 0;
}

