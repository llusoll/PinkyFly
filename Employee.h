#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <vector>

enum class EmployeeRole { FlightAttendant, Dispatcher, Admin };

class Employee {
public:
    std::string employeeId;
    std::string firstName;
    std::string lastName;
    EmployeeRole role;
    std::string login;
    std::string passwordHash;

    Employee() = default;
    Employee(const std::string& id, const std::string& fn, const std::string& ln,
        EmployeeRole r, const std::string& log, const std::string& pwd);

    static std::string roleToString(EmployeeRole r);
    static EmployeeRole stringToRole(const std::string& s);
    bool authenticate(const std::string& pwd) const;

    static void loadAll(std::vector<Employee>& employees);
    static void saveAll(const std::vector<Employee>& employees);
};

#endif