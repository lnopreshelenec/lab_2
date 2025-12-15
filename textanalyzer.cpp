#include "textanalyzer.h"
#include <QDebug>
#include <QByteArray>

TextAnalyzer::TextAnalyzer() : words(nullptr), wordCount(0), capacity(10) {
    words = new char*[capacity];
    for (int i = 0; i < capacity; i++) {
        words[i] = nullptr;
    }
    qDebug() << "Конструктор TextAnalyzer";
}

TextAnalyzer::TextAnalyzer(const TextAnalyzer& other)
    : wordCount(other.wordCount), capacity(other.capacity) {

    words = new char*[capacity];
    for (int i = 0; i < capacity; i++) {
        if (i < wordCount && other.words[i]) {
            words[i] = copyString(other.words[i]);
        } else {
            words[i] = nullptr;
        }
    }
    qDebug() << "Конструктор копирования TextAnalyzer";
}

TextAnalyzer::~TextAnalyzer() {
    for (int i = 0; i < wordCount; i++) {
        if (words[i]) {
            delete[] words[i];
        }
    }
    delete[] words;
    qDebug() << "Деструктор TextAnalyzer";
}

TextAnalyzer& TextAnalyzer::operator=(const TextAnalyzer& other) {
    if (this != &other) {
        // Освобождаем старую память
        for (int i = 0; i < wordCount; i++) {
            if (words[i]) {
                delete[] words[i];
            }
        }
        delete[] words;

        wordCount = other.wordCount;
        capacity = other.capacity;

        words = new char*[capacity];
        for (int i = 0; i < capacity; i++) {
            if (i < wordCount && other.words[i]) {
                words[i] = copyString(other.words[i]);
            } else {
                words[i] = nullptr;
            }
        }
    }
    return *this;
}

void TextAnalyzer::resize(int newCapacity) {
    char** newWords = new char*[newCapacity];

    for (int i = 0; i < wordCount; i++) {
        newWords[i] = words[i];
    }
    for (int i = wordCount; i < newCapacity; i++) {
        newWords[i] = nullptr;
    }

    delete[] words;
    words = newWords;
    capacity = newCapacity;
}

// Статический метод для копирования строки
char* TextAnalyzer::copyString(const char* source) {
    if (!source) return nullptr;

    int length = 0;
    while (source[length] != '\0') {
        length++;
    }

    char* result = new char[length + 1];
    for (int i = 0; i <= length; i++) {
        result[i] = source[i];
    }

    return result;
}

int TextAnalyzer::stringLength(const char* str) const {
    if (!str) return 0;

    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

bool TextAnalyzer::stringsEqual(const char* str1, const char* str2) const {
    if (!str1 || !str2) return false;

    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == str2[i];
}

void TextAnalyzer::addWord(const char* word) {
    if (!word) return;

    if (wordCount >= capacity) {
        resize(capacity * 2);
    }

    words[wordCount] = copyString(word);
    wordCount++;
}

void TextAnalyzer::analyzeText(const QString& text) {
    // Очищаем старые данные
    for (int i = 0; i < wordCount; i++) {
        if (words[i]) {
            delete[] words[i];
            words[i] = nullptr;
        }
    }
    wordCount = 0;

    QByteArray byteArray = text.toLocal8Bit();
    const char* textData = byteArray.constData();

    if (!textData || textData[0] == '\0') {
        throw TextAnalyzerException("Текст пустой");
    }

    char buffer[256];
    int bufferIndex = 0;

    for (int i = 0; textData[i] != '\0'; i++) {
        char c = textData[i];

        // Буквы, цифры и русские буквы
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= 'А' && c <= 'я') || (c >= '0' && c <= '9')) {

            if (bufferIndex < 255) {
                buffer[bufferIndex++] = c;
            }
        } else if (bufferIndex > 0) {
            // Конец слова
            buffer[bufferIndex] = '\0';
            addWord(buffer);
            bufferIndex = 0;
        }
    }

    // Последнее слово
    if (bufferIndex > 0) {
        buffer[bufferIndex] = '\0';
        addWord(buffer);
    }

    if (wordCount == 0) {
        throw TextAnalyzerException("В тексте не найдено слов");
    }
}

char* TextAnalyzer::findLongestWord(int& count) const {
    if (wordCount == 0) {
        throw TextAnalyzerException("Нет слов для анализа");
    }

    // Находим максимальную длину
    int maxLength = 0;
    for (int i = 0; i < wordCount; i++) {
        int length = stringLength(words[i]);
        if (length > maxLength) {
            maxLength = length;
        }
    }

    // Считаем количество слов максимальной длины
    count = 0;
    char* firstLongest = nullptr;

    for (int i = 0; i < wordCount; i++) {
        if (stringLength(words[i]) == maxLength) {
            count++;
            if (!firstLongest) {
                firstLongest = copyString(words[i]); // Теперь это статический метод
            }
        }
    }

    return firstLongest;
}

int TextAnalyzer::getWordCount() const {
    return wordCount;
}
