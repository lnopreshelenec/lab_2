#ifndef TEXTANALYZER_H
#define TEXTANALYZER_H

#include <QString>
#include <exception>

class TextAnalyzerException : public std::exception {
private:
    QString message;

public:
    TextAnalyzerException(const QString& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.toLocal8Bit().constData();
    }
    QString getMessage() const { return message; }
};

class TextAnalyzer {
private:
    char** words;
    int wordCount;
    int capacity;

    void resize(int newCapacity);
    void addWord(const char* word);
    int stringLength(const char* str) const;
    bool stringsEqual(const char* str1, const char* str2) const;

    // Вспомогательная статическая функция для копирования строки
    static char* copyString(const char* source);

public:
    TextAnalyzer();
    TextAnalyzer(const TextAnalyzer& other);
    ~TextAnalyzer();

    TextAnalyzer& operator=(const TextAnalyzer& other);

    void analyzeText(const QString& text);
    char* findLongestWord(int& count) const;
    int getWordCount() const;
};

#endif
