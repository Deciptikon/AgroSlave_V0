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
    gps->setMsecUpdate(1000);

    threadGPS = new QThread(this);

    gps->moveToThread(threadGPS);
    ///----------------------------------------------------------------------------------------------



    ///-------Connects objects-----------------------------------------------------------------------
    connect(gps      , SIGNAL(updatePositionXY(const double&, const double&)),
            autopilot, SLOT(readFromGPS(const double&, const double&)) );
    //connect(gps, SIGNAL(updatePositionGeo(QGeoCoordinate&)), this, SLOT(updateLatLon(QGeoCoordinate&)));
    ///----------------------------------------------------------------------------------------------



    ///-------Start threads--------------------------------------------------------------------------
    threadAutopilot->start();
    threadGPS->start();
    ///----------------------------------------------------------------------------------------------
}

MainWindow::~MainWindow()
{
    delete ui;
}

