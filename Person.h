#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <stdexcept>

class Person {
public:
    std::string firstName;
    std::string lastName;


    // Конструкторы
    Person() = default;
    Person(const std::string& fn, const std::string& ln);

    // Конструктор копирования
    Person(const Person& other);

    // Оператор присваивания
    Person& operator=(const Person& other);

    // Геттеры (публичные, т.к. protected поля не доступны извне)
    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }

    // Валидация имени
    static void validateName(const std::string& name);

    // Перегрузка оператора сравнения
    bool operator==(const Person& other) const;
    bool operator!=(const Person& other) const;

    // Дружественная функция для вывода
    friend std::ostream& operator<<(std::ostream& os, const Person& p);
};

#endif 