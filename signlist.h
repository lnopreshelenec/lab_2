#ifndef SIGNLIST_H
#define SIGNLIST_H

#include "sign.h"

class SignList {
private:
    SIGN** data;
    int capacity;
    int size;

    void resize(int newCapacity);

public:
    SignList();
    ~SignList();

    void add(SIGN* sign);
    SIGN* get(int index) const;
    int getSize() const;
    void sortByBirthDate();
    void displayAll() const;
    void clear();

    // Простой линейный поиск
    int findByLastName(const QString& lastName) const;
    void findByZodiac(const QString& zodiac) const;
};

#endif
