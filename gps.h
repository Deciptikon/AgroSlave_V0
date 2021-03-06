#ifndef GPS_H
#define GPS_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
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
    void updatePositionXY(const double &x, const double &y);
    void updatePositionLatLon(const double &lat, const double &lon);

    void readyRead(const QByteArray &bytes);
    void parseMessage();

public slots:
    void init();
    void write(const QByteArray &bytes);

private slots:
    void readPort();
    void ubxParser();
private:
    void latLonToXY(double lat, double lon);



    QSerialPort* serial;

    QByteArray messageBuffer;
    QByteArray messageCurrent;

    double lat;
    double lon;

    double x,y;

    size_t msecUpdate;

};

#endif // GPS_H
