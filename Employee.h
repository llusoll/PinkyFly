#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Person.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

enum class EmployeeRole { FlightAttendant, Dispatcher, Admin };

class Employee : public Person {
public:
    std::string employeeId;
    EmployeeRole role;
    std::string login;
    std::string passwordHash;

    static int totalEmployees;

    Employee() = default;
    Employee(const std::string& id, const std::string& fn, const std::string& ln, EmployeeRole r, const std::string& log, const std::string& pwd);
    Employee(const Employee& other) = delete;
    Employee& operator=(const Employee& other) = delete;

    static std::string roleToString(EmployeeRole r);
    static EmployeeRole stringToRole(const std::string& s);
    bool authenticate(const std::string& pwd) const;
    static void validateEmployee(const std::string& id, const std::string& login);

    const std::string& getId() const { return employeeId; }
    EmployeeRole getRole() const { return role; }
    const std::string& getLogin() const { return login; }

    bool operator==(const Employee& other) const;
    bool operator!=(const Employee& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Employee& e);

    static void loadAll(std::vector<std::unique_ptr<Employee>>& employees);
    static void saveAll(const std::vector<std::unique_ptr<Employee>>& employees);
};

#endif
