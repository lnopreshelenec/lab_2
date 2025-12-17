#include "signlist.h"
#include <QDebug>
#include <algorithm>

SignList::SignList() : data(nullptr), capacity(10), size(0) {
    data = new SIGN*[capacity];
    for (int i = 0; i < capacity; i++) {
        data[i] = nullptr;
    }
    qDebug() << "Создан SignList с вместимостью" << capacity;
}

SignList::SignList(const SignList& other) : data(nullptr), capacity(0), size(0) {
    deepCopy(other);
    qDebug() << "Вызван конструктор копирования SignList";
}

SignList::~SignList() {
    clear();
    delete[] data;
    data = nullptr;
    qDebug() << "Уничтожен SignList";
}

SignList& SignList::operator=(const SignList& other) {
    if (this != &other) {
        clear();
        delete[] data;
        deepCopy(other);
    }
    return *this;
}

void SignList::deepCopy(const SignList& other) {
    capacity = other.capacity;
    size = other.size;
    data = new SIGN*[capacity];

    for (int i = 0; i < capacity; i++) {
        if (i < size && other.data[i]) {
            data[i] = new SIGN(*other.data[i]); // Глубокое копирование объекта
        } else {
            data[i] = nullptr;
        }
    }
}

void SignList::resize(int newCapacity) {
    if (newCapacity <= size) return;

    SIGN** newData = new SIGN*[newCapacity];

    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }

    for (int i = size; i < newCapacity; i++) {
        newData[i] = nullptr;
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

void SignList::add(SIGN* sign) {
    if (!sign) return;

    if (size >= capacity) {
        resize(capacity * 2);
    }

    data[size] = sign;
    size++;
}

SIGN* SignList::get(int index) const {
    if (index < 0 || index >= size) {
        throw SignException("Некорректный индекс");
    }
    return data[index];
}

int SignList::getSize() const {
    return size;
}

void SignList::sortByBirthDate() {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (*data[j + 1] < *data[j]) {
                SIGN* temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

void SignList::clear() {
    for (int i = 0; i < size; i++) {
        if (data[i]) {
            delete data[i];
            data[i] = nullptr;
        }
    }
    size = 0;
}

void SignList::replace(int index, SIGN* newSign) {
    if (index < 0 || index >= size) {
        throw SignException("Некорректный индекс для замены");
    }

    if (!newSign) {
        throw SignException("Новый элемент не может быть nullptr");
    }

    if (data[index]) {
        delete data[index];
    }

    data[index] = newSign;
}

void SignList::remove(int index) {
    if (index < 0 || index >= size) {
        throw SignException("Некорректный индекс для удаления");
    }
    qDebug()<<data;
    if (data[index] != nullptr) {
        delete data[index];
    }

    // Сдвигаем элементы
    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }

    data[size - 1] = nullptr;
    size--;
}


int SignList::findByLastName(const QString& lastName) const {
    for (int i = 0; i < size; i++) {
        if (data[i]->getLastName().toLower() == lastName.toLower()) {
            return i;
        }
    }
    return -1;
}

