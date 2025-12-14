#ifndef NOTEWINDOW_H
#define NOTEWINDOW_H

#include <QMainWindow>
#include "notemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class NoteWindow;
}
QT_END_NAMESPACE

class NoteWindow : public QMainWindow
{
    Q_OBJECT

public:
    NoteWindow(QWidget *parent = nullptr);
    ~NoteWindow();

private slots:
    void onAddNote();
    void onEditNote();
    void onRemoveNote();
    void onSearchNote();
    void onSaveToFile();
    void onLoadFromFile();
    void onClearAll();
    void onNoteSelected(int row, int column);
    void updateDisplay();

private:
    Ui::NoteWindow *ui;
    NoteManager manager;
    int currentEditIndex;

    void clearInputFields();
    void showNoteDetails(const NOTE& note);
    void populateEditFields(const NOTE& note);
};

#endif // NOTEWINDOW_H
