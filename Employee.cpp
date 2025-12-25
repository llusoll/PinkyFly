#include "Employee.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

int Employee::totalEmployees = 0;

void Employee::validateEmployee(const std::string& id, const std::string& login) {
    if (id.empty()) throw std::invalid_argument("ID сотрудника не может быть пустым.");
    if (login.empty()) throw std::invalid_argument("Логин не может быть пустым.");
}

Employee::Employee(const std::string& id, const std::string& fn, const std::string& ln, EmployeeRole r, const std::string& log, const std::string& pwd)
    : Person(fn, ln), employeeId(id), role(r), login(log) {
    validateEmployee(id, log);
    passwordHash = simpleHash(pwd);
    ++totalEmployees;
}



bool Employee::operator==(const Employee& other) const {
    return this->employeeId == other.employeeId;
}

bool Employee::operator!=(const Employee& other) const {
    return !(*this == other);
}

std::string Employee::roleToString(EmployeeRole r) {
    switch (r) {
    case EmployeeRole::Admin: return "Администратор";
    case EmployeeRole::Dispatcher: return "Диспетчер";
    case EmployeeRole::FlightAttendant: return "Бортпроводник";
    default: return "Бортпроводник";
    }
}

EmployeeRole Employee::stringToRole(const std::string& s) {
    if (s == "Администратор") return EmployeeRole::Admin;
    if (s == "Диспетчер") return EmployeeRole::Dispatcher;
    if (s == "Бортпроводник") return EmployeeRole::FlightAttendant;
    return EmployeeRole::FlightAttendant;
}

bool Employee::authenticate(const std::string& pwd) const {
    return simpleHash(pwd) == this->passwordHash; // явное использование this
}

std::ostream& operator<<(std::ostream& os, const Employee& e) {
    os << "Сотрудник[ID=" << e.employeeId << ", ФИО=" << static_cast<const Person&>(e)
        << ", Роль=" << Employee::roleToString(e.role) << ", Логин=" << e.login << "]";
    return os;
}

void Employee::loadAll(std::vector<std::unique_ptr<Employee>>& employees) {
    employees.clear();
    std::ifstream file("data/employees.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        line.pop_back();
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() != 6) continue;

        try {
            auto e = std::make_unique<Employee>();
            e->employeeId = parts[0];
            e->firstName = parts[1];
            e->lastName = parts[2];
            e->role = stringToRole(parts[3]);
            e->login = parts[4];
            e->passwordHash = parts[5];
            validateEmployee(e->employeeId, e->login);
            employees.push_back(std::move(e));
        }
        catch (const std::exception& ex) {
            std::cerr << "Пропущена некорректная запись сотрудника:" << ex.what() << "\n";
        }
    }
    file.close();
}

void Employee::saveAll(const std::vector<std::unique_ptr<Employee>>& employees) {
    ensureDataDir();
    std::ofstream file("data/employees.txt");
    file << "# ID;FirstName;LastName;Role;Login;PasswordHash\n";
    for (const auto& e : employees) {
        file << e->employeeId << ";"
            << e->firstName << ";"
            << e->lastName << ";"
            << roleToString(e->role) << ";"
            << e->login << ";"
            << e->passwordHash << "\n";
    }
    file.close();
}
