#include "textconverter.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

TextConverter::TextConverter() {
    // Инициализация соответствия цифр словам
    digitToWord['0'] = "ноль";
    digitToWord['1'] = "один";
    digitToWord['2'] = "два";
    digitToWord['3'] = "три";
    digitToWord['4'] = "четыре";
    digitToWord['5'] = "пять";
    digitToWord['6'] = "шесть";
    digitToWord['7'] = "семь";
    digitToWord['8'] = "восемь";
    digitToWord['9'] = "девять";
}

QString TextConverter::convertText(const QString& text) {
    QString result;
    bool newSentence = true;

    for (int i = 0; i < text.length(); ++i) {
        QChar ch = text[i];

        // Начинаем новое предложение с новой строки
        if (newSentence && !ch.isSpace()) {
            if (!result.isEmpty() && !result.endsWith('\n')) {
                result += '\n';
            }
            newSentence = false;
        }

        // Проверяем, является ли символ цифрой
        if (digitToWord.contains(ch)) {
            result += digitToWord[ch];
        } else {
            result += ch;
        }

        // Проверяем конец предложения
        if (ch == '.' || ch == '!' || ch == '?') {
            newSentence = true;
        }
    }

    return result;
}

QString TextConverter::readAndConvertFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString("Ошибка: Не удалось открыть файл %1").arg(filename);
    }

    QTextStream in(&file);
    QString text = in.readAll();
    file.close();

    return convertText(text);
}
