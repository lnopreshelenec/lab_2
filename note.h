#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QDate>
#include <iostream>

class NOTE {
private:
    QString firstName;
    QString lastName;
    QString phoneNumber;
    int birthDay;
    int birthMonth;
    int birthYear;

public:
    // Конструкторы
    NOTE();
    NOTE(const QString& firstName, const QString& lastName,
         const QString& phoneNumber, int day, int month, int year);

    // Методы доступа
    QString getFirstName() const;
    QString getLastName() const;
    QString getPhoneNumber() const;
    QDate getBirthDate() const;
    int getBirthDay() const;
    int getBirthMonth() const;
    int getBirthYear() const;

    void setFirstName(const QString& name);
    void setLastName(const QString& name);
    void setPhoneNumber(const QString& phone);
    void setBirthDate(int day, int month, int year);

    // Перегрузка операторов ввода/вывода
    friend std::ostream& operator<<(std::ostream& os, const NOTE& note);
    friend std::istream& operator>>(std::istream& is, NOTE& note);

    // Для сортировки по первым трем цифрам
    QString getPhonePrefix() const;

    // Операторы сравнения для сортировки
    bool operator<(const NOTE& other) const;
    bool operator==(const NOTE& other) const;

    // Преобразование в строку для отображения
    QString toString() const;
    QString toFormattedString() const;

    // Проверка по фамилии
    bool hasLastName(const QString& lastName) const;
};

#endif // NOTE_H
