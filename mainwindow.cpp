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
    gps = new GPS(this);
    gps->init();
    //gps->setMsecUpdate(100);

    threadGPS = new QThread(this);

    gps->moveToThread(threadGPS);
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
    //connect(gps, SIGNAL(updatePositionGeo(QGeoCoordinate&)), this, SLOT(updateLatLon(QGeoCoordinate&)));
    //connect(ui->bt_send_read_command, SIGNAL(clicked()), slavecontroller_14, SLOT(writeData()));
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

