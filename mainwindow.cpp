#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем список
    signList = new SignList();

    // Настройка таблицы
    setupTable();

    // Настройка фильтра по знакам
    QStringList zodiacSigns = {"Овен", "Телец", "Близнецы", "Рак",
                               "Лев", "Дева", "Весы", "Скорпион",
                               "Стрелец", "Козерог", "Водолей", "Рыбы"};
    ui->filterComboBox->addItems(zodiacSigns);

    // Добавляем тестовые данные
    addSampleData();

    // Обновление таблицы
    updateTable();

    // Устанавливаем контекстное меню для таблицы
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested,
            this, &MainWindow::showMenu);
}

MainWindow::~MainWindow()
{
    delete signList;
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

    // Включаем контекстное меню
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::addSampleData()
{
    try {
        // Добавляем несколько тестовых записей
        signList->add(new SIGN("Иван", "Иванов", 15, 3, 1990));
        signList->add(new SIGN("Петр", "Петров", 25, 4, 1985));
        signList->add(new SIGN("Мария", "Сидорова", 10, 6, 1995));
        signList->add(new SIGN("Анна", "Смирнова", 5, 7, 1992));
        signList->add(new SIGN("Алексей", "Козлов", 23, 8, 1985));
    } catch (const SignException& e) {
        showMessage("Ошибка", e.getMessage());
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

void MainWindow::showMessage(const QString& title, const QString& message)
{
    QMessageBox::information(this, title, message);
}

// Поиск по фамилии
void MainWindow::on_pushButton_clicked()
{
    QString lastName = ui->searchLineEdit->text().trimmed();

    if (lastName.isEmpty()) {
        showMessage("Ошибка", "Введите фамилию для поиска");
        return;
    }

    int index = signList->findByLastName(lastName);

    if (index == -1) {
        showMessage("Результат поиска",
                    QString("Записей с фамилией '%1' не найдено.").arg(lastName));
    } else {
        SIGN* foundSign = signList->get(index);
        showMessage("Найдено",
                    QString("Найден человек:\n\n%1").arg(foundSign->toString()));

        // Подсвечиваем найденную строку в таблице
        ui->tableWidget->selectRow(index);
    }
}

// Поиск по знаку зодиака
void MainWindow::on_pushButton_2_clicked()
{
    QString zodiac = ui->filterComboBox->currentText();

    // Выводим результаты в консоль
    signList->findByZodiac(zodiac);

    // Показываем сообщение пользователю
    showMessage("Поиск по знаку",
                QString("Результаты поиска по знаку '%1' выведены в консоль.").arg(zodiac));
}

// Сортировка по дате рождения
void MainWindow::on_pushButton_3_clicked()
{
    signList->sortByBirthDate();
    updateTable();
    showMessage("Сортировка", "Записи отсортированы по дате рождения");

    // Также выводим в консоль для демонстрации
    std::cout << "\n=== После сортировки ===" << std::endl;
    signList->displayAll();
}

// Двойной клик по таблице
void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    if (row >= 0 && row < signList->getSize()) {
        SIGN* sign = signList->get(row);
        showMessage("Информация", sign->toString());
    }
}

// Контекстное меню для дополнительных операций
void MainWindow::showMenu()
{
    QMenu menu(this);

    menu.addAction("Добавить запись", this, [this]() {
        bool ok;
        QString firstName = QInputDialog::getText(this, "Добавить запись",
                                                  "Введите имя:", QLineEdit::Normal, "", &ok);
        if (!ok || firstName.isEmpty()) return;

        QString lastName = QInputDialog::getText(this, "Добавить запись",
                                                 "Введите фамилию:", QLineEdit::Normal, "", &ok);
        if (!ok || lastName.isEmpty()) return;

        int day = QInputDialog::getInt(this, "Добавить запись",
                                       "Введите день рождения:", 1, 1, 31, 1, &ok);
        if (!ok) return;

        int month = QInputDialog::getInt(this, "Добавить запись",
                                         "Введите месяц рождения:", 1, 1, 12, 1, &ok);
        if (!ok) return;

        int year = QInputDialog::getInt(this, "Добавить запись",
                                        "Введите год рождения:", 2000, 1900, 2024, 1, &ok);
        if (!ok) return;

        try {
            signList->add(new SIGN(firstName, lastName, day, month, year));
            updateTable();
            showMessage("Успех", "Запись успешно добавлена");
        } catch (const SignException& e) {
            showMessage("Ошибка", e.getMessage());
        }
    });

    menu.addAction("Добавить из консоли", this, [this]() {
        std::cout << "\n=== Добавление новой записи ===" << std::endl;

        try {
            SIGN newSign;
            std::cin >> newSign;
            signList->add(new SIGN(newSign));
            updateTable();
            showMessage("Успех", "Запись добавлена из консоли");
        } catch (const SignException& e) {
            showMessage("Ошибка", e.getMessage());
        }
    });

    menu.addSeparator();

    menu.addAction("Показать все в консоли", this, [this]() {
        signList->displayAll();
        showMessage("Информация", "Все записи выведены в консоль");
    });

    menu.addAction("Очистить список", this, [this]() {
        int answer = QMessageBox::question(this, "Подтверждение",
                                           "Вы уверены, что хотите очистить весь список?");
        if (answer == QMessageBox::Yes) {
            signList->clear();
            updateTable();
            showMessage("Успех", "Список очищен");
        }
    });

    menu.exec(QCursor::pos());
}
