#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QObject>
#include <QThread>
#include <QTimer>
#include <viewdata.h>
#include "drawtrack.h"
#include "autopilot.h"
#include "gps.h"
#include "controlleri2c.h"

typedef QList<QVector2D> ListVector;

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    ViewData viewData;//модель с данными для отображения

    Autopilot *autopilot;
    QThread *threadAutopilot;
    QTimer *timerAutopilot;

    GPS *gps;
    QThread *threadGPS;

    ControllerI2C *controlleri2c_14;
    QThread *threadControllerI2C_14;

    QQmlApplicationEngine engine;

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("ViewData", &viewData);

    qmlRegisterType<DrawTrack>("DrawTrack",1,0,"DrawTrack");
    qRegisterMetaType<ListVector>("ListVector");


    ///-------Create autopilot and move to thread with timer----------------------------------------
    autopilot = new Autopilot();
    autopilot->init(1000);

    threadAutopilot = new QThread();

    timerAutopilot = new QTimer(0);
    timerAutopilot->setInterval(autopilot->getMSecDeltaTime());
    timerAutopilot->moveToThread(threadAutopilot);

    autopilot->connect( timerAutopilot, SIGNAL(timeout()), SLOT(loop()), Qt::ConnectionType::DirectConnection);
    timerAutopilot->connect(threadAutopilot, SIGNAL(started()), SLOT(start()));
    ///----------------------------------------------------------------------------------------------



    ///-------Create GPS-reader----------------------------------------------------------------------
    gps = new GPS();

    threadGPS = new QThread();

    gps->moveToThread(threadGPS);
    gps->connect(threadGPS, SIGNAL(started()), SLOT(init()) );

    viewData.connect(gps, SIGNAL(updatePositionXY(const double&, const double&)),
                     SLOT(acceptCoord(const double&, const double&)), Qt::QueuedConnection );
    ///----------------------------------------------------------------------------------------------



    ///-------Create I2C slave device----------------------------------------------------------------
    controlleri2c_14 = new ControllerI2C();
    controlleri2c_14->init( 0x14 );

    threadControllerI2C_14 = new QThread();
    controlleri2c_14->moveToThread(threadControllerI2C_14);
    ///----------------------------------------------------------------------------------------------



    ///-------Connects objects-----------------------------------------------------------------------
    autopilot->connect(gps      , SIGNAL(updatePositionXY(const double&, const double&)),
                                  SLOT(readFromGPS(const double&, const double&)) );
    autopilot->connect(&viewData, SIGNAL(sendKeyPointForAdding(const QVector2D&)),
                                  SLOT(addKeyPoint(const QVector2D&)) );

    viewData.connect(autopilot, SIGNAL(pathChanged(const ListVector&)),
                                SLOT(acceptPath(const ListVector&)) );
    viewData.connect(autopilot, SIGNAL(keyPointsChanged(const ListVector&)),
                                SLOT(acceptKeyPoints(const ListVector&)) );

    controlleri2c_14->connect(autopilot, SIGNAL(sendCommandToSlave14(const int&)),
                                         SLOT(writeData(const int&)) );
    controlleri2c_14->connect(&viewData, SIGNAL(signalCommandToSlave14(const int&)),
                                         SLOT(writeData(const int&)) );

    ///----------------------------------------------------------------------------------------------


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    ///-------Start threads--------------------------------------------------------------------------
    threadAutopilot->start();
    threadGPS->start();
    threadControllerI2C_14->start();
    ///----------------------------------------------------------------------------------------------


    return app.exec();
}
