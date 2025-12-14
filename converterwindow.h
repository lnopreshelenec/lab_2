#ifndef CONVERTERWINDOW_H
#define CONVERTERWINDOW_H

#include <QMainWindow>
#include "textconverter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ConverterWindow;
}
QT_END_NAMESPACE

class ConverterWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConverterWindow(QWidget *parent = nullptr);
    ~ConverterWindow();

private slots:
    void onConvertText();
    void onLoadAndConvertFile();
    void onSaveResult();

private:
    Ui::ConverterWindow *ui;
    TextConverter converter;
};

#endif // CONVERTERWINDOW_H
