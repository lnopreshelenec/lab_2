#include "notewindow.h"
#include "ui_notewindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>

NoteWindow::NoteWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NoteWindow)
    , currentEditIndex(-1)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"№", "ФИО", "Телефон", "Дата рождения"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    // Подключение сигналов
    connect(ui->addButton, &QPushButton::clicked, this, &NoteWindow::onAddNote);
    connect(ui->editButton, &QPushButton::clicked, this, &NoteWindow::onEditNote);
    connect(ui->removeButton, &QPushButton::clicked, this, &NoteWindow::onRemoveNote);
    connect(ui->searchButton, &QPushButton::clicked, this, &NoteWindow::onSearchNote);
    connect(ui->saveButton, &QPushButton::clicked, this, &NoteWindow::onSaveToFile);
    connect(ui->loadButton, &QPushButton::clicked, this, &NoteWindow::onLoadFromFile);
    connect(ui->clearButton, &QPushButton::clicked, this, &NoteWindow::onClearAll);
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &NoteWindow::onNoteSelected);

    updateDisplay();
}

NoteWindow::~NoteWindow()
{
    delete ui;
}

void NoteWindow::onAddNote()
{
    QString firstName = ui->firstNameEdit->text().trimmed();
    QString lastName = ui->lastNameEdit->text().trimmed();
    QString phone = ui->phoneEdit->text().trimmed();
    int day = ui->daySpinBox->value();
    int month = ui->monthSpinBox->value();
    int year = ui->yearSpinBox->value();

    if (firstName.isEmpty() || lastName.isEmpty() || phone.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все обязательные поля");
        return;
    }

    NOTE note(firstName, lastName, phone, day, month, year);

    if (currentEditIndex >= 0) {
        // Редактирование существующей записи
        manager.editNote(currentEditIndex, note);
        currentEditIndex = -1;
        ui->addButton->setText("Добавить");
    } else {
        // Добавление новой записи
        int position = ui->positionSpinBox->value() - 1;
        if (position < 0 || position > manager.getNoteCount()) {
            manager.addNote(note);
        } else {
            manager.addNoteAt(position, note);
        }
    }

    clearInputFields();
    updateDisplay();
    QMessageBox::information(this, "Успех", "Запись сохранена");
}

void NoteWindow::onEditNote()
{
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow >= 0 && selectedRow < manager.getNoteCount()) {
        currentEditIndex = selectedRow;
        NOTE note = manager.getNote(selectedRow);
        populateEditFields(note);
        ui->addButton->setText("Сохранить изменения");
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите запись для редактирования");
    }
}

void NoteWindow::onRemoveNote()
{
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow >= 0 && selectedRow < manager.getNoteCount()) {
        manager.removeNote(selectedRow);
        updateDisplay();
        QMessageBox::information(this, "Успех", "Запись удалена");
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите запись для удаления");
    }
}

void NoteWindow::onSearchNote()
{
    QString lastName = ui->searchEdit->text().trimmed();
    if (lastName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите фамилию для поиска");
        return;
    }

    QVector<NOTE> foundNotes = manager.findByLastName(lastName);

    if (foundNotes.isEmpty()) {
        QMessageBox::information(this, "Результат поиска",
                                 "Записей с такой фамилией не найдено");
    } else {
        QString result = "Найдено записей: " + QString::number(foundNotes.size()) + "\n\n";
        for (const auto& note : foundNotes) {
            result += note.toFormattedString() + "\n";
        }

        QMessageBox::information(this, "Результат поиска", result);
    }
}

void NoteWindow::onSaveToFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "Текстовые файлы (*.txt)");
    if (!filename.isEmpty()) {
        manager.saveToFile(filename);
        QMessageBox::information(this, "Успех", "Данные сохранены в файл");
    }
}

void NoteWindow::onLoadFromFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Загрузить файл", "", "Текстовые файлы (*.txt)");
    if (!filename.isEmpty()) {
        manager.loadFromFile(filename);
        updateDisplay();
        QMessageBox::information(this, "Успех", "Данные загружены из файла");
    }
}

void NoteWindow::onClearAll()
{
    if (QMessageBox::question(this, "Подтверждение",
                              "Вы уверены, что хотите удалить все записи?") == QMessageBox::Yes) {
        manager.clear();
        updateDisplay();
        QMessageBox::information(this, "Успех", "Все записи удалены");
    }
}

void NoteWindow::onNoteSelected(int row, int column)
{
    Q_UNUSED(column);
    if (row >= 0 && row < manager.getNoteCount()) {
        NOTE note = manager.getNote(row);
        showNoteDetails(note);
    }
}

void NoteWindow::updateDisplay()
{
    ui->tableWidget->setRowCount(0);

    QVector<NOTE> notes = manager.getAllNotes();
    for (int i = 0; i < notes.size(); ++i) {
        const NOTE& note = notes[i];

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(note.getLastName() + " " + note.getFirstName()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(note.getPhoneNumber()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(
                                             QString("%1.%2.%3")
                                                 .arg(note.getBirthDay(), 2, 10, QChar('0'))
                                                 .arg(note.getBirthMonth(), 2, 10, QChar('0'))
                                                 .arg(note.getBirthYear())));
    }

    ui->tableWidget->resizeColumnsToContents();
}

void NoteWindow::clearInputFields()
{
    ui->firstNameEdit->clear();
    ui->lastNameEdit->clear();
    ui->phoneEdit->clear();
    ui->daySpinBox->setValue(1);
    ui->monthSpinBox->setValue(1);
    ui->yearSpinBox->setValue(2000);
    ui->positionSpinBox->setValue(1);
    ui->searchEdit->clear();
}

void NoteWindow::showNoteDetails(const NOTE& note)
{
    ui->detailsTextEdit->setText(note.toFormattedString());
}

void NoteWindow::populateEditFields(const NOTE& note)
{
    ui->firstNameEdit->setText(note.getFirstName());
    ui->lastNameEdit->setText(note.getLastName());
    ui->phoneEdit->setText(note.getPhoneNumber());
    ui->daySpinBox->setValue(note.getBirthDay());
    ui->monthSpinBox->setValue(note.getBirthMonth());
    ui->yearSpinBox->setValue(note.getBirthYear());
}
