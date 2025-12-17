#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>
#include <QDate>

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

    // Добавляем тестовые данные
    addSampleData();

    // Обновление таблицы
    updateTable();
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

    // Включаем выбор строк
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

SIGN* MainWindow::createSignFromDialog(bool editMode, SIGN* originalSign)
{
    bool ok;

    // Устанавливаем значения по умолчанию
    QString defaultFirstName = editMode ? originalSign->getFirstName() : "";
    QString defaultLastName = editMode ? originalSign->getLastName() : "";
    int defaultDay = editMode ? originalSign->getBirthDay() : 1;
    int defaultMonth = editMode ? originalSign->getBirthMonth() : 1;
    int defaultYear = editMode ? originalSign->getBirthYear() : 2000;

    // Диалог для ввода имени
    QString firstName = QInputDialog::getText(this,
                                              editMode ? "Редактирование записи" : "Добавление записи",
                                              "Введите имя:",
                                              QLineEdit::Normal,
                                              defaultFirstName,
                                              &ok);

    if (!ok || firstName.isEmpty()) {
        throw SignException("Имя не может быть пустым");
    }

    // Диалог для ввода фамилии
    QString lastName = QInputDialog::getText(this,
                                             editMode ? "Редактирование записи" : "Добавление записи",
                                             "Введите фамилию:",
                                             QLineEdit::Normal,
                                             defaultLastName,
                                             &ok);

    if (!ok || lastName.isEmpty()) {
        throw SignException("Фамилия не может быть пустой");
    }

    // Диалог для ввода дня
    int day = QInputDialog::getInt(this,
                                   editMode ? "Редактирование записи" : "Добавление записи",
                                   "Введите день рождения (1-31):",
                                   defaultDay,
                                   1, 31, 1, &ok);

    if (!ok) {
        throw SignException("День рождения не указан");
    }

    // Диалог для ввода месяца
    int month = QInputDialog::getInt(this,
                                     editMode ? "Редактирование записи" : "Добавление записи",
                                     "Введите месяц рождения (1-12):",
                                     defaultMonth,
                                     1, 12, 1, &ok);

    if (!ok) {
        throw SignException("Месяц рождения не указан");
    }

    // Диалог для ввода года
    int year = QInputDialog::getInt(this,
                                    editMode ? "Редактирование записи" : "Добавление записи",
                                    "Введите год рождения:",
                                    defaultYear,
                                    1900, QDate::currentDate().year(), 1, &ok);

    if (!ok) {
        throw SignException("Год рождения не указан");
    }

    // Создаем новый объект SIGN
    return new SIGN(firstName, lastName, day, month, year);
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

// Сортировка по дате рождения
void MainWindow::on_pushButton_3_clicked()
{
    signList->sortByBirthDate();
    updateTable();
    showInfoMessage("Записи отсортированы по дате рождения");

    // // Также выводим в консоль для демонстрации
    // std::cout << "\n=== После сортировки ===" << std::endl;
    // signList->displayAll();
}

// Добавить запись (кнопка)
void MainWindow::on_addButton_clicked()
{
    try {
        SIGN* newSign = createSignFromDialog(false);
        signList->add(newSign);
        updateTable();
        showInfoMessage("Запись успешно добавлена в конец списка");
    } catch (const SignException& e) {
        showErrorMessage(e.getMessage());
    }
}

// Добавить на позицию (кнопка)
// Добавить на позицию (кнопка)
void MainWindow::on_addAtPositionButton_clicked()
{
    int currentSize = signList->getSize();
    if (currentSize == 0) {
        on_addButton_clicked();
        return;
    }

    bool ok;
    int position = QInputDialog::getInt(this, "Добавление записи",
                                        "Введите позицию для добавления (1 - " +
                                            QString::number(currentSize + 1) + "):",
                                        1, 1, currentSize + 1, 1, &ok);

    if (!ok) return;

    try {
        SIGN* newSign = createSignFromDialog(false);

        // Создаем новый список
        SignList* tempList = new SignList();

        // Добавляем элементы до указанной позиции (создаем копии)
        for (int i = 0; i < position - 1; i++) {
            SIGN* originalSign = signList->get(i);
            SIGN* copySign = new SIGN(originalSign->getFirstName(),
                                      originalSign->getLastName(),
                                      originalSign->getBirthDay(),
                                      originalSign->getBirthMonth(),
                                      originalSign->getBirthYear());
            tempList->add(copySign);
        }

        // Добавляем новый элемент
        tempList->add(newSign);

        // Добавляем остальные элементы (создаем копии)
        for (int i = position - 1; i < currentSize; i++) {
            SIGN* originalSign = signList->get(i);
            SIGN* copySign = new SIGN(originalSign->getFirstName(),
                                      originalSign->getLastName(),
                                      originalSign->getBirthDay(),
                                      originalSign->getBirthMonth(),
                                      originalSign->getBirthYear());
            tempList->add(copySign);
        }

        // Заменяем старый список новым
        delete signList;
        signList = tempList;

        updateTable();
        showInfoMessage(QString("Запись успешно добавлена на позицию %1").arg(position));

    } catch (const SignException& e) {
        showErrorMessage(e.getMessage());
    }
}

// Редактировать запись (кнопка)
void MainWindow::on_editButton_clicked()
{
    int selectedRow = ui->tableWidget->currentRow();

    if (selectedRow < 0 || selectedRow >= signList->getSize()) {
        showErrorMessage("Выберите запись для редактирования");
        return;
    }

    try {
        SIGN* originalSign = signList->get(selectedRow);
        SIGN* editedSign = createSignFromDialog(true, originalSign);

        // Заменяем старую запись
        signList->replace(selectedRow, editedSign);

        updateTable();
        showInfoMessage("Запись успешно отредактирована");

    } catch (const SignException& e) {
        showErrorMessage(e.getMessage());
    }
}


// Удалить запись (кнопка)
void MainWindow::on_deleteButton_clicked()
{
    // Проверяем, есть ли записи
    if (signList->getSize() == 0) {
        showErrorMessage("Нет записей для удаления");
        return;
    }

    int selectedRow = ui->tableWidget->currentRow();
    qDebug() << "Удаление: selectedRow =" << selectedRow << ", size =" << signList->getSize();

    if (selectedRow < 0 || selectedRow >= signList->getSize()) {
        showErrorMessage("Выберите запись для удаления");
        return;
    }

    try {
        SIGN* signToDelete = signList->get(selectedRow);
        if (!signToDelete) {
            showErrorMessage("Ошибка: запись не найдена");
            return;
        }

        QString fullName = signToDelete->getLastName() + " " + signToDelete->getFirstName();

        int answer = QMessageBox::question(this, "Подтверждение удаления",
                                           "Вы уверены, что хотите удалить запись:\n" + fullName + "?",
                                           QMessageBox::Yes | QMessageBox::No);

        if (answer == QMessageBox::Yes) {
            signList->remove(selectedRow);
            updateTable();
            showInfoMessage("Запись успешно удалена");
        }

    } catch (const SignException& e) {
        showErrorMessage(e.getMessage());
    }
}

// Очистить все (кнопка)
void MainWindow::on_clearButton_clicked()
{
    if (signList->getSize() == 0) {
        showInfoMessage("Список уже пуст");
        return;
    }

    int answer = QMessageBox::question(this, "Подтверждение очистки",
                                       "Вы уверены, что хотите удалить все записи?\n"
                                       "Это действие нельзя отменить.",
                                       QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes) {
        signList->clear();
        updateTable();
        showInfoMessage("Все записи удалены");
    }
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

        // Анализируем текст
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
            // Выводим результат
            ui->longestWordLineEdit->setText(longestWord);
            ui->countLineEdit->setText(QString::number(count));

            // Освобождаем память
            delete[] longestWord;
        }

    } catch (const TextAnalyzerException& e) {
        showErrorMessage(e.getMessage());
    }
}

// void MainWindow::clearTextFields()
// {
//     ui->longestWordLineEdit->clear();
//     ui->countLineEdit->clear();

//     delete textAnalyzer;
//     textAnalyzer = new TextAnalyzer();

//     showInfoMessage("Поля текста очищены");
// }

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
