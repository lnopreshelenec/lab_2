#ifndef SIGNLIST_H
#define SIGNLIST_H

#include "sign.h"

class SignList {
private:
    SIGN** data = nullptr;
    int capacity;
    int size;

    void resize(int newCapacity);
    void deepCopy(const SignList& other); // Новый метод для глубокого копирования

public:
    SignList();
    SignList(const SignList& other);
    ~SignList();

    SignList& operator=(const SignList& other);

    void add(SIGN* sign); // Принимает владение объектом
    SIGN* get(int index) const;
    int getSize() const;
    void sortByBirthDate();
    void displayAll() const;
    void clear();
    void replace(int index, SIGN* newSign); // Принимает владение новым объектом
    void remove(int index);
    SIGN* take(int index); // Берет объект без удаления

    int findByLastName(const QString& lastName) const;
    void findByZodiac(const QString& zodiac) const;
};

#endif
