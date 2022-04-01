#include "gps.h"

GPS::GPS(QObject *parent) : QObject(parent)
{
    msecUpdate = 1000;
}

void GPS::setMsecUpdate(size_t value)
{
    if(value < 0) {
        return;
    }
    if(value < 10000) {
        msecUpdate = value;
    }
}

void GPS::init()
{
    serial = new QSerialPort();
    serial->setPortName("ttyUSB0");//"ttyACM0"
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if(!serial->isOpen()) {
        //Если порт не открыт, попытка через 100мс открыть его снова
        QTimer::singleShot(100, this, SLOT(init()));
        serial->open(QIODevice::ReadWrite);
    }
    if(serial->isOpen()) {
        qDebug() << "SerialPort is open";

        source = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode);
        source->setDevice(serial);

        if(source){
            connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdate(QGeoPositionInfo)));
            source->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
            source->setUpdateInterval(msecUpdate);
            source->startUpdates();
            qDebug() << "GPS ready and updating";
        }
    } else {
        qDebug() << "SerialPort is not open";
    }

}

void GPS::positionUpdate(QGeoPositionInfo info)
{
    qDebug() << "SIGNAL(positionUpdate)";

    currentCoordinate = info.coordinate();
    latLonToXY(currentCoordinate.latitude(), currentCoordinate.longitude());

    qDebug() << "coord.latitude(): " << QString::number(currentCoordinate.latitude(), 'g', 9);
    qDebug() << "coord.longitude(): " << QString::number(currentCoordinate.longitude(), 'g', 9);

    QGeoCoordinate c = info.coordinate();
    double lat = c.latitude();
    double lon = c.longitude();
    emit updatePositionGeo(c);
    emit updatePositionXY(this->x, this->y);
    emit updatePositionLatLon(lat, lon);
}

void GPS::latLonToXY(double lat, double lon)
{
    if(lat>360 || lat<0) {
        qDebug() << "Latitude not range";
    }
    if(lon>MAX_LONGITUDE || lon<-MAX_LONGITUDE) {
        qDebug() << "Longitude not range";
    }

    this->y = RADIUS_EARTH*(lon + 180.0)*(M_PI/180.0);
    this->x = RADIUS_EARTH*qLn(qTan(M_PI/4 + lat*(M_PI/180.0)/2.0));
}
