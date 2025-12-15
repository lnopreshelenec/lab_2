#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем список знаков зодиака
    signList = new SignList();

    // Создаем анализатор текста
    textAnalyzer = new TextAnalyzer();

    // Настройка таблицы
    setupTable();

    // Настройка фильтра по знакам
    QStringList zodiacSigns = {"Овен", "Телец", "Близнецы", "Рак",
                               "Лев", "Дева", "Весы", "Скорпион",
                               "Стрелец", "Козерог", "Водолей", "Рыбы"};
    // ui->filterComboBox->addItems(zodiacSigns);

    // Добавляем тестовые данные
    addSampleData();

    // Обновление таблицы
    updateTable();

    // Скрываем вкладку работы с файлом (если не настроена в UI)
    // ui->tabWidget->setTabEnabled(1, false);
}

MainWindow::~MainWindow()
{
    delete signList;
    delete textAnalyzer;
    delete ui;
}

void MainWindow::setupTable()
{
    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"Фамилия", "Имя", "Знак зодиака", "Дата рождения"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 150);
    ui->tableWidget->setColumnWidth(3, 150);
}

void MainWindow::addSampleData()
{
    try {
        signList->add(new SIGN("Иван", "Иванов", 15, 3, 1990));
        signList->add(new SIGN("Петр", "Петров", 25, 4, 1985));
        signList->add(new SIGN("Мария", "Сидорова", 10, 6, 1995));
        signList->add(new SIGN("Анна", "Смирнова", 5, 7, 1992));
        signList->add(new SIGN("Алексей", "Козлов", 23, 8, 1985));
    } catch (const SignException& e) {
        showErrorMessage(e.getMessage());
    }
}

void MainWindow::updateTable()
{
    // Очищаем таблицу
    ui->tableWidget->setRowCount(0);

    // Заполняем таблицу данными
    for (int i = 0; i < signList->getSize(); i++) {
        SIGN* sign = signList->get(i);

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        // Добавляем данные в таблицу
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(sign->getLastName()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(sign->getFirstName()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(sign->getZodiacSign()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(sign->getBirthDateString()));
    }
}

void MainWindow::showInfoMessage(const QString& message)
{
    QMessageBox::information(this, "Информация", message);
}

void MainWindow::showErrorMessage(const QString& message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

// Поиск по фамилии
void MainWindow::on_pushButton_clicked()
{
    QString lastName = ui->searchLineEdit->text().trimmed();

    if (lastName.isEmpty()) {
        showErrorMessage("Введите фамилию для поиска");
        return;
    }

    int index = signList->findByLastName(lastName);

    if (index == -1) {
        showInfoMessage(QString("Записей с фамилией '%1' не найдено.").arg(lastName));
    } else {
        SIGN* foundSign = signList->get(index);
        showInfoMessage(QString("Найден человек:\n\n%1").arg(foundSign->toString()));

        // Подсвечиваем найденную строку в таблице
        ui->tableWidget->selectRow(index);
    }
}

// Поиск по знаку зодиака
void MainWindow::on_pushButton_2_clicked()
{
    // QString zodiac = ui->filterComboBox->currentText();

    // Выводим результаты в консоль
    // signList->findByZodiac(zodiac);

    // Показываем сообщение пользователю
    // showInfoMessage(QString("Результаты поиска по знаку '%1' выведены в консоль.").arg(zodiac));
}

// Сортировка по дате рождения
void MainWindow::on_pushButton_3_clicked()
{
    signList->sortByBirthDate();
    updateTable();
    showInfoMessage("Записи отсортированы по дате рождения");

    // Также выводим в консоль для демонстрации
    std::cout << "\n=== После сортировки ===" << std::endl;
    signList->displayAll();
}

// Загрузка файла для анализа текста
void MainWindow::on_loadButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Открыть текстовый файл",
                                                    "",
                                                    "Текстовые файлы (*.txt);;Все файлы (*.*)"
                                                    );

    if (filename.isEmpty()) {
        return;
    }

    try {
        QString content = readFileContent(filename);

        // Проверяем, есть ли текстовое поле для отображения содержимого
        // Если нет, просто анализируем текст
        textAnalyzer->analyzeText(content);

        // Выводим информацию о загрузке
        showInfoMessage(QString("Файл '%1' успешно загружен и проанализирован.\n"
                                "Всего слов: %2")
                            .arg(QFileInfo(filename).fileName())
                            .arg(textAnalyzer->getWordCount()));

    } catch (const TextAnalyzerException& e) {
        showErrorMessage(e.getMessage());
    }
}

// Анализ текста
void MainWindow::on_analyzeButton_clicked()
{
    try {
        // Проверяем, был ли загружен текст
        if (textAnalyzer->getWordCount() == 0) {
            showErrorMessage("Сначала загрузите текстовый файл");
            return;
        }

        // Находим самое длинное слово
        int count = 0;
        char* longestWord = textAnalyzer->findLongestWord(count);

        if (longestWord) {
            // Выводим результат (предполагаем, что есть соответствующие поля в UI)
            // Если их нет, выводим в сообщении
            ui->countLineEdit->setText(QString::number(count));
            ui->longestWordLineEdit->setText(longestWord);
            // ui->analyzeText->setText();


            // QString result = QString("Результаты анализа:\n"
            //                          "• Самое длинное слово: '%1'\n"
            //                          "• Встречается в тексте: %2 раз(а)\n"
            //                          "• Всего слов в тексте: %3")
            //                      .arg(longestWord)
            //                      .arg(count)
            //                      .arg(textAnalyzer->getWordCount());

            // showInfoMessage(result);

            // Освобождаем память
            delete[] longestWord;
        }

    } catch (const TextAnalyzerException& e) {
        showErrorMessage(e.getMessage());
    }
}

QString MainWindow::readFileContent(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw TextAnalyzerException("Не удалось открыть файл для чтения");
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    if (content.isEmpty()) {
        throw TextAnalyzerException("Файл пустой");
    }

    return content;
}
