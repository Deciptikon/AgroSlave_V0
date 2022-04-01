#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>

#include "autopilot.h"
#include "gps.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Autopilot* autopilot;
    QThread* threadAutopilot;
    QTimer* timerAutopilot;

    GPS* gps;
    QThread* threadGPS;
};
#endif // MAINWINDOW_H
