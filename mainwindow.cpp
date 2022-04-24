#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ///-------Create autopilot and move to thread with timer----------------------------------------
    autopilot = new Autopilot(this);
    autopilot->init(1000);

    threadAutopilot = new QThread(this);

    timerAutopilot = new QTimer(0);
    timerAutopilot->setInterval(autopilot->getMSecDeltaTime());
    timerAutopilot->moveToThread(threadAutopilot);

    autopilot->connect( timerAutopilot, SIGNAL(timeout()), SLOT(loop()), Qt::ConnectionType::DirectConnection);
    timerAutopilot->connect(threadAutopilot, SIGNAL(started()), SLOT(start()));
    ///----------------------------------------------------------------------------------------------



    ///-------Create GPS-reader----------------------------------------------------------------------
    gps = new GPS();
    //gps->init();
    //gps->setMsecUpdate(100);

    threadGPS = new QThread(this);

    gps->moveToThread(threadGPS);
    connect(threadGPS, SIGNAL(started()), gps, SLOT(init()));
    ///----------------------------------------------------------------------------------------------



    ///-------Create I2C slave device----------------------------------------------------------------
    controlleri2c_14 = new ControllerI2C();
    controlleri2c_14->init( 0x14 );

    threadControllerI2C_14 = new QThread(this);
    controlleri2c_14->moveToThread(threadControllerI2C_14);
    ///----------------------------------------------------------------------------------------------



    ///-------Connects objects-----------------------------------------------------------------------
    connect(gps      , SIGNAL(updatePositionXY(const double&, const double&)),
            autopilot, SLOT(readFromGPS(const double&, const double&)) );
    connect(gps, SIGNAL(updatePositionLatLon(const double&, const double&)),
            this, SLOT(updateLabelLatLon(const double&, const double&)) );
    connect(ui->bt_create_list_point, SIGNAL(clicked()), autopilot, SLOT(createListPoint()));
    connect(autopilot, SIGNAL(sendCommandToSlave14(int&)), controlleri2c_14, SLOT(writeData(int&)));
    ///----------------------------------------------------------------------------------------------



    ///-------Start threads--------------------------------------------------------------------------
    threadAutopilot->start();
    threadGPS->start();
    threadControllerI2C_14->start();
    ///----------------------------------------------------------------------------------------------
}

MainWindow::~MainWindow()
{
    delete ui;
    delete autopilot;
    delete threadAutopilot;
    delete timerAutopilot;

    delete gps;
    delete threadGPS;

    delete controlleri2c_14;
    delete threadControllerI2C_14;
}

void MainWindow::updateLabelLatLon(const double &lat, const double &lon)
{
    this->ui->lb_latitude->setText( QString::number(lat, 'd', 7));
    this->ui->lb_longitude->setText(QString::number(lon, 'd', 7));
}



void MainWindow::on_bt_restate_relay_clicked()
{
    int d = 120;
    controlleri2c_14->writeData(d);
}

