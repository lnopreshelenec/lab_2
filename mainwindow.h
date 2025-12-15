#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "signlist.h"

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

    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void showMenu();

private:
    Ui::MainWindow *ui;
    SignList* signList;

    void setupTable();
    void updateTable();
    void showMessage(const QString& title, const QString& message);
    void addSampleData();
};

#endif
