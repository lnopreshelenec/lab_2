#ifndef SIGN_H
#define SIGN_H

#include <QString>
#include <iostream>
#include <exception>

class SignException : public std::exception {
private:
    QString message;

public:
    SignException(const QString& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.toLocal8Bit().constData();
    }
    QString getMessage() const { return message; }
};

class SIGN {
private:
    QString* firstName;
    QString* lastName;
    QString* zodiacSign;
    int birthDay;
    int birthMonth;
    int birthYear;

    void calculateZodiacSign();
    bool isValidDate(int day, int month, int year) const;

public:
    SIGN();
    SIGN(const QString& firstName, const QString& lastName,
         int day, int month, int year);
    SIGN(const SIGN& other);
    ~SIGN();

    SIGN& operator=(const SIGN& other);
    bool operator<(const SIGN& other) const;
    bool operator==(const SIGN& other) const;
    friend std::ostream& operator<<(std::ostream& os, const SIGN& sign);
    friend std::istream& operator>>(std::istream& is, SIGN& sign);

    void setFirstName(const QString& name);
    void setLastName(const QString& name);
    void setBirthDate(int day, int month, int year);

    QString getFirstName() const;
    QString getLastName() const;
    QString getZodiacSign() const;
    int getBirthDay() const;
    int getBirthMonth() const;
    int getBirthYear() const;
    QString getBirthDateString() const;

    void display() const;
    QString toString() const;
};

#endif
