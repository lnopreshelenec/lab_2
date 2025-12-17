#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "signlist.h"
#include "textanalyzer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked(); // Поиск по фамилии
    void on_pushButton_3_clicked(); // Сортировка по дате
    void on_loadButton_clicked(); // Загрузка файла для анализа
    void on_analyzeButton_clicked(); // Анализ текста

    // Слоты для кнопок управления записями
    void on_addButton_clicked(); // Добавить запись
    void on_addAtPositionButton_clicked(); // Добавить на позицию
    void on_editButton_clicked(); // Редактировать запись
    void on_deleteButton_clicked(); // Удалить запись
    void on_clearButton_clicked(); // Очистить все

    // Слоты для текстового анализа
    // void clearTextFields();

private:
    Ui::MainWindow *ui;
    SignList* signList;
    TextAnalyzer* textAnalyzer;

    void setupTable();
    void updateTable();
    void showInfoMessage(const QString& message);
    void showErrorMessage(const QString& message);
    void addSampleData();
    QString readFileContent(const QString& filename);

    // Вспомогательные методы
    SIGN* createSignFromDialog(bool editMode = false, SIGN* originalSign = nullptr);
};

#endif // MAINWINDOW_H
