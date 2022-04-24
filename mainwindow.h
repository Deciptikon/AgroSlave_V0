#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>

#include "autopilot.h"
#include "gps.h"
#include "controlleri2c.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateLabelLatLon(const double &lat, const double &lon);

    void on_bt_restate_relay_clicked();

private:
    Ui::MainWindow *ui;

    Autopilot* autopilot;
    QThread* threadAutopilot;
    QTimer* timerAutopilot;

    GPS* gps;
    QThread* threadGPS;

    ControllerI2C* controlleri2c_14;
    QThread* threadControllerI2C_14;
};
#endif // MAINWINDOW_H