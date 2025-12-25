#include "Employee.h"
#include <fstream>
#include "Utils.h"

Employee::Employee(const std::string& id, const std::string& fn, const std::string& ln,
    EmployeeRole r, const std::string& log, const std::string& pwd)
    : employeeId(id), firstName(fn), lastName(ln), role(r), login(log) {
    passwordHash = simpleHash(pwd);
}

std::string Employee::roleToString(EmployeeRole r) {
    switch (r) {
    case EmployeeRole::Admin: return "Admin";
    case EmployeeRole::Dispatcher: return "Dispatcher";
    case EmployeeRole::FlightAttendant: return "FlightAttendant";
    default: return "FlightAttendant";
    }
}

EmployeeRole Employee::stringToRole(const std::string& s) {
    if (s == "Admin") return EmployeeRole::Admin;
    if (s == "Dispatcher") return EmployeeRole::Dispatcher;
    if (s == "FlightAttendant") return EmployeeRole::FlightAttendant;
    return EmployeeRole::FlightAttendant;
}

bool Employee::authenticate(const std::string& pwd) const {
    return simpleHash(pwd) == passwordHash;
}

void Employee::loadAll(std::vector<Employee>& employees) {
    employees.clear();
    std::ifstream file("data/employees.txt");
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto parts = split(line, ';');
        if (parts.size() != 6) continue;
        Employee e;
        e.employeeId = parts[0];
        e.firstName = parts[1];
        e.lastName = parts[2];
        e.role = stringToRole(parts[3]);
        e.login = parts[4];
        e.passwordHash = parts[5];
        employees.push_back(e);
    }
    file.close();
}

void Employee::saveAll(const std::vector<Employee>& employees) {
    ensureDataDir();
    std::ofstream file("data/employees.txt");
    file << "# ID;FirstName;LastName;Role;Login;PasswordHash\n";
    for (const auto& e : employees) {
        file << e.employeeId << ";"
            << e.firstName << ";"
            << e.lastName << ";"
            << roleToString(e.role) << ";"
            << e.login << ";"
            << e.passwordHash << "\n";
    }
    file.close();
}