#include "Person.h"
#include <iostream>
#include <algorithm>
#include <cctype>

void Person::validateName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Имя не может быть пустым.");
    }
    if (!std::all_of(name.begin(), name.end(), [](unsigned char c) {
        return std::isalpha(c) || c == '-' || c == ' ';
        })) {
        throw std::invalid_argument("Имя содержит недопустимые символы.");
    }
}

Person::Person(const std::string& fn, const std::string& ln)
    : firstName(fn), lastName(ln) {
    validateName(firstName);
    validateName(lastName);
}

Person::Person(const Person& other)
    : firstName(other.firstName), lastName(other.lastName) {
}

Person& Person::operator=(const Person& other) {
    if (this == &other) return *this;
    firstName = other.firstName;
    lastName = other.lastName;
    return *this;
}

bool Person::operator==(const Person& other) const {
    return firstName == other.firstName && lastName == other.lastName;
}

bool Person::operator!=(const Person& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << p.lastName << " " << p.firstName;
    return os;
}