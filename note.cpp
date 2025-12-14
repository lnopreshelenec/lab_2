#include "note.h"
#include <iostream>
#include <sstream>
#include <iomanip>

NOTE::NOTE()
    : firstName(""), lastName(""), phoneNumber(""),
    birthDay(1), birthMonth(1), birthYear(2000) {}

NOTE::NOTE(const QString& firstName, const QString& lastName,
           const QString& phoneNumber, int day, int month, int year)
    : firstName(firstName), lastName(lastName), phoneNumber(phoneNumber),
    birthDay(day), birthMonth(month), birthYear(year) {}

// Методы доступа
QString NOTE::getFirstName() const { return firstName; }
QString NOTE::getLastName() const { return lastName; }
QString NOTE::getPhoneNumber() const { return phoneNumber; }

int NOTE::getBirthDay() const { return birthDay; }
int NOTE::getBirthMonth() const { return birthMonth; }
int NOTE::getBirthYear() const { return birthYear; }

QDate NOTE::getBirthDate() const {
    return QDate(birthYear, birthMonth, birthDay);
}

void NOTE::setFirstName(const QString& name) { firstName = name; }
void NOTE::setLastName(const QString& name) { lastName = name; }
void NOTE::setPhoneNumber(const QString& phone) { phoneNumber = phone; }
void NOTE::setBirthDate(int day, int month, int year) {
    birthDay = day;
    birthMonth = month;
    birthYear = year;
}

// Получение первых трех цифр телефона
QString NOTE::getPhonePrefix() const {
    if (phoneNumber.length() >= 3) {
        return phoneNumber.left(3);
    }
    return phoneNumber;
}

// Операторы сравнения для сортировки
bool NOTE::operator<(const NOTE& other) const {
    return getPhonePrefix() < other.getPhonePrefix();
}

bool NOTE::operator==(const NOTE& other) const {
    return (lastName == other.lastName &&
            firstName == other.firstName &&
            phoneNumber == other.phoneNumber);
}

// Преобразование в строку
QString NOTE::toString() const {
    return QString("%1 %2 | Телефон: %3 | Дата рождения: %4.%5.%6")
        .arg(lastName)
        .arg(firstName)
        .arg(phoneNumber)
        .arg(birthDay, 2, 10, QChar('0'))
        .arg(birthMonth, 2, 10, QChar('0'))
        .arg(birthYear);
}

QString NOTE::toFormattedString() const {
    return QString("Фамилия: %1\nИмя: %2\nТелефон: %3\nДата рождения: %4.%5.%6\n")
        .arg(lastName)
        .arg(firstName)
        .arg(phoneNumber)
        .arg(birthDay, 2, 10, QChar('0'))
        .arg(birthMonth, 2, 10, QChar('0'))
        .arg(birthYear);
}

// Проверка по фамилии
bool NOTE::hasLastName(const QString& searchLastName) const {
    return lastName.toLower() == searchLastName.toLower();
}

// Перегрузка оператора вывода
std::ostream& operator<<(std::ostream& os, const NOTE& note) {
    os << note.lastName.toStdString() << " "
       << note.firstName.toStdString() << " "
       << note.phoneNumber.toStdString() << " "
       << note.birthDay << " "
       << note.birthMonth << " "
       << note.birthYear;
    return os;
}

// Перегрузка оператора ввода
std::istream& operator>>(std::istream& is, NOTE& note) {
    std::string lastName, firstName, phone;
    int day, month, year;

    if (is >> lastName >> firstName >> phone >> day >> month >> year) {
        note.lastName = QString::fromStdString(lastName);
        note.firstName = QString::fromStdString(firstName);
        note.phoneNumber = QString::fromStdString(phone);
        note.birthDay = day;
        note.birthMonth = month;
        note.birthYear = year;
    }
    return is;
}
