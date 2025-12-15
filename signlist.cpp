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

SignList::~SignList() {
    clear();
    delete[] data;
    qDebug() << "Уничтожен SignList";
}

void SignList::resize(int newCapacity) {
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

    qDebug() << "SignList увеличен до" << newCapacity;
}

void SignList::add(SIGN* sign) {
    if (size >= capacity) {
        resize(capacity * 2);
    }

    data[size] = sign;
    size++;
    qDebug() << "Добавлен элемент в SignList. Текущий размер:" << size;
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

// Пузырьковая сортировка
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
    qDebug() << "SignList отсортирован по дате рождения";
}

void SignList::displayAll() const {
    if (size == 0) {
        std::cout << "Список пуст." << std::endl;
        return;
    }

    std::cout << "\n=== Все записи (" << size << " шт.) ===" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << i + 1 << ". ";
        data[i]->display();
    }
    std::cout << "======================================" << std::endl;
}

void SignList::clear() {
    for (int i = 0; i < size; i++) {
        delete data[i];
    }
    size = 0;
}

int SignList::findByLastName(const QString& lastName) const {
    for (int i = 0; i < size; i++) {
        if (data[i]->getLastName().toLower() == lastName.toLower()) {
            return i;
        }
    }
    return -1;
}

void SignList::findByZodiac(const QString& zodiac) const {
    std::cout << "\n=== Люди со знаком '" << zodiac.toStdString() << "' ===" << std::endl;
    bool found = false;

    for (int i = 0; i < size; i++) {
        if (data[i]->getZodiacSign() == zodiac) {
            std::cout << i + 1 << ". ";
            data[i]->display();
            found = true;
        }
    }

    if (!found) {
        std::cout << "Записей не найдено." << std::endl;
    }
}
