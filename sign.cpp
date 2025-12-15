#include "sign.h"
#include <QDebug>
#include <QDate>

SIGN::SIGN() {
    firstName = new QString("Неизвестно");
    lastName = new QString("Неизвестно");
    zodiacSign = new QString("Неизвестно");
    birthDay = 1;
    birthMonth = 1;
    birthYear = 2000;

    qDebug() << "Вызван конструктор без параметров для объекта SIGN";
}

SIGN::SIGN(const QString& firstName, const QString& lastName,
           int day, int month, int year) {

    if (!isValidDate(day, month, year)) {
        throw SignException("Некорректная дата рождения");
    }

    this->firstName = new QString(firstName);
    this->lastName = new QString(lastName);
    this->zodiacSign = new QString();
    this->birthDay = day;
    this->birthMonth = month;
    this->birthYear = year;

    calculateZodiacSign();

    qDebug() << "Вызван конструктор с параметрами для объекта SIGN: "
             << *this->lastName << " " << *this->firstName;
}

SIGN::SIGN(const SIGN& other) {
    firstName = new QString(*other.firstName);
    lastName = new QString(*other.lastName);
    zodiacSign = new QString(*other.zodiacSign);
    birthDay = other.birthDay;
    birthMonth = other.birthMonth;
    birthYear = other.birthYear;

    qDebug() << "Вызван конструктор копирования для объекта SIGN: "
             << *lastName << " " << *firstName;
}

SIGN::~SIGN() {
    delete firstName;
    delete lastName;
    delete zodiacSign;

    qDebug() << "Вызван деструктор для объекта SIGN: "
             << *lastName << " " << *firstName;
}

SIGN& SIGN::operator=(const SIGN& other) {
    if (this != &other) {
        *firstName = *other.firstName;
        *lastName = *other.lastName;
        *zodiacSign = *other.zodiacSign;
        birthDay = other.birthDay;
        birthMonth = other.birthMonth;
        birthYear = other.birthYear;
    }
    return *this;
}

bool SIGN::operator<(const SIGN& other) const {
    if (birthMonth != other.birthMonth)
        return birthMonth < other.birthMonth;
    return birthDay < other.birthDay;
}

bool SIGN::operator==(const SIGN& other) const {
    return (*lastName == *other.lastName &&
            *firstName == *other.firstName &&
            birthDay == other.birthDay &&
            birthMonth == other.birthMonth &&
            birthYear == other.birthYear);
}

bool SIGN::isValidDate(int day, int month, int year) const {
    if (year < 1900 || year > QDate::currentDate().year())
        return false;
    if (month < 1 || month > 12)
        return false;

    QDate date(year, month, day);
    return date.isValid();
}

void SIGN::calculateZodiacSign() {
    if ((birthMonth == 3 && birthDay >= 21) || (birthMonth == 4 && birthDay <= 20))
        *zodiacSign = "Овен";
    else if ((birthMonth == 4 && birthDay >= 21) || (birthMonth == 5 && birthDay <= 20))
        *zodiacSign = "Телец";
    else if ((birthMonth == 5 && birthDay >= 21) || (birthMonth == 6 && birthDay <= 21))
        *zodiacSign = "Близнецы";
    else if ((birthMonth == 6 && birthDay >= 22) || (birthMonth == 7 && birthDay <= 22))
        *zodiacSign = "Рак";
    else if ((birthMonth == 7 && birthDay >= 23) || (birthMonth == 8 && birthDay <= 23))
        *zodiacSign = "Лев";
    else if ((birthMonth == 8 && birthDay >= 24) || (birthMonth == 9 && birthDay <= 22))
        *zodiacSign = "Дева";
    else if ((birthMonth == 9 && birthDay >= 23) || (birthMonth == 10 && birthDay <= 23))
        *zodiacSign = "Весы";
    else if ((birthMonth == 10 && birthDay >= 24) || (birthMonth == 11 && birthDay <= 22))
        *zodiacSign = "Скорпион";
    else if ((birthMonth == 11 && birthDay >= 23) || (birthMonth == 12 && birthDay <= 21))
        *zodiacSign = "Стрелец";
    else if ((birthMonth == 12 && birthDay >= 22) || (birthMonth == 1 && birthDay <= 20))
        *zodiacSign = "Козерог";
    else if ((birthMonth == 1 && birthDay >= 21) || (birthMonth == 2 && birthDay <= 19))
        *zodiacSign = "Водолей";
    else if ((birthMonth == 2 && birthDay >= 20) || (birthMonth == 3 && birthDay <= 20))
        *zodiacSign = "Рыбы";
    else
        *zodiacSign = "Неизвестно";
}

void SIGN::setFirstName(const QString& name) {
    if (name.isEmpty()) {
        throw SignException("Имя не может быть пустым");
    }
    *firstName = name;
}

void SIGN::setLastName(const QString& name) {
    if (name.isEmpty()) {
        throw SignException("Фамилия не может быть пустой");
    }
    *lastName = name;
}

void SIGN::setBirthDate(int day, int month, int year) {
    if (!isValidDate(day, month, year)) {
        throw SignException("Некорректная дата рождения");
    }

    birthDay = day;
    birthMonth = month;
    birthYear = year;
    calculateZodiacSign();
}

QString SIGN::getFirstName() const { return *firstName; }
QString SIGN::getLastName() const { return *lastName; }
QString SIGN::getZodiacSign() const { return *zodiacSign; }
int SIGN::getBirthDay() const { return birthDay; }
int SIGN::getBirthMonth() const { return birthMonth; }
int SIGN::getBirthYear() const { return birthYear; }

QString SIGN::getBirthDateString() const {
    return QString("%1.%2.%3")
    .arg(birthDay, 2, 10, QChar('0'))
        .arg(birthMonth, 2, 10, QChar('0'))
        .arg(birthYear);
}

void SIGN::display() const {
    std::cout << toString().toStdString() << std::endl;
}

QString SIGN::toString() const {
    return QString("%1 %2 | Знак зодиака: %3 | Дата рождения: %4")
        .arg(*lastName)
        .arg(*firstName)
        .arg(*zodiacSign)
        .arg(getBirthDateString());
}

std::ostream& operator<<(std::ostream& os, const SIGN& sign) {
    os << sign.getLastName().toStdString() << " "
       << sign.getFirstName().toStdString() << " "
       << sign.getZodiacSign().toStdString() << " "
       << sign.getBirthDay() << " "
       << sign.getBirthMonth() << " "
       << sign.getBirthYear();
    return os;
}

std::istream& operator>>(std::istream& is, SIGN& sign) {
    std::string lastName, firstName, zodiac;
    int day, month, year;

    std::cout << "Введите фамилию: ";
    is >> lastName;
    std::cout << "Введите имя: ";
    is >> firstName;
    std::cout << "Введите день рождения: ";
    is >> day;
    std::cout << "Введите месяц рождения: ";
    is >> month;
    std::cout << "Введите год рождения: ";
    is >> year;

    sign.setLastName(QString::fromStdString(lastName));
    sign.setFirstName(QString::fromStdString(firstName));
    sign.setBirthDate(day, month, year);

    return is;
}
