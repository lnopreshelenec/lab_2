#include "converterwindow.h"
#include "ui_converterwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

ConverterWindow::ConverterWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConverterWindow)
{
    ui->setupUi(this);

    connect(ui->convertButton, &QPushButton::clicked, this, &ConverterWindow::onConvertText);
    connect(ui->loadFileButton, &QPushButton::clicked, this, &ConverterWindow::onLoadAndConvertFile);
    connect(ui->saveButton, &QPushButton::clicked, this, &ConverterWindow::onSaveResult);
}

ConverterWindow::~ConverterWindow()
{
    delete ui;
}

void ConverterWindow::onConvertText()
{
    QString inputText = ui->inputTextEdit->toPlainText();
    if (inputText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите текст для преобразования");
        return;
    }

    QString convertedText = converter.convertText(inputText);
    ui->outputTextEdit->setPlainText(convertedText);
}

void ConverterWindow::onLoadAndConvertFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Текстовые файлы (*.txt)");
    if (!filename.isEmpty()) {
        QString convertedText = converter.readAndConvertFile(filename);
        ui->outputTextEdit->setPlainText(convertedText);
        QMessageBox::information(this, "Успех", "Файл загружен и преобразован");
    }
}

void ConverterWindow::onSaveResult()
{
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить результат", "", "Текстовые файлы (*.txt)");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << ui->outputTextEdit->toPlainText();
            file.close();
            QMessageBox::information(this, "Успех", "Результат сохранен");
        }
    }
}
