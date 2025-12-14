#ifndef TEXTCONVERTER_H
#define TEXTCONVERTER_H

#include <QString>
#include <QMap>

class TextConverter {
private:
    QMap<QChar, QString> digitToWord;

public:
    TextConverter();
    QString convertText(const QString& text);
    QString readAndConvertFile(const QString& filename);
};

#endif // TEXTCONVERTER_H
