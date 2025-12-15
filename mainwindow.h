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
    void on_pushButton_2_clicked(); // Поиск по знаку
    void on_pushButton_3_clicked(); // Сортировка по дате
    void on_loadButton_clicked(); // Загрузка файла для анализа
    void on_analyzeButton_clicked(); // Анализ текста

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
};

#endif // MAINWINDOW_H
