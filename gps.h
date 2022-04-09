#ifndef GPS_H
#define GPS_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QGeoPositionInfoSource>
#include <QNmeaPositionInfoSource>
#include <QtMath>
#include <QTimer>


const double RADIUS_EARTH = 6371000;
const double MAX_LONGITUDE = 85.0511288;

class GPS : public QObject
{
    Q_OBJECT
public:
    explicit GPS(QObject *parent = nullptr);

    void setMsecUpdate(size_t msec);


signals:
    void updatePositionGeo(QGeoCoordinate &coord);
    void updatePositionXY(const double &x, const double &y);
    void updatePositionLatLon(double &Lat, double &Lon);

    void readyRead(const QByteArray &bytes);

public slots:
    void init();
    void write(const QByteArray &bytes);

private slots:
    void positionUpdate(QGeoPositionInfo info);
    void readPort();
    void ubxParser(const QByteArray &bytes);
private:
    void latLonToXY(double lat, double lon);



    QSerialPort* serial;

    QGeoCoordinate currentCoordinate;

    double x,y;

    size_t msecUpdate;

};

#endif // GPS_H
