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
        serial->open(QIODevice::ReadWrite);
    }
    if(serial->isOpen()) {
        qDebug() << "SerialPort is open";
        connect(serial, SIGNAL(readyRead()), this, SLOT(readPort()));
        connect(this, SIGNAL(readyRead(const QByteArray &)), this, SLOT(ubxParser(const QByteArray &)));
    } else {
        qDebug() << "SerialPort is not open";
        //Если порт не открыт, попытка через 100мс открыть его снова
        QTimer::singleShot(100, this, SLOT(init()));
    }

}

void GPS::write(const QByteArray &bytes)
{
    qDebug() << "SerialPort writing";
    if(bytes.isEmpty()) {
        return;
    }
    if(!serial->isOpen()) {
        return;
    }
    if(!serial->isWritable()) {
        return;
    }

    serial->write(bytes);

    if(*bytes.end() == '\n') {
        return;
    }

    QByteArray b;
    b.append('\n');
    serial->write(b);
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

void GPS::readPort()
{
    //qDebug() << "SerialPort: SIGNAL(readyRead())";
    if(!serial->canReadLine()) {
        return;
    }

    QByteArray bytes;
    bytes = serial->readLine();

    if(bytes.isEmpty()) {
        qDebug() << "SerialPort read not data";
        return;
    }

//    QString s="";
//    for(auto b: bytes) {
//        s.append(b);
//    }
//    qDebug() << "SerialPort read: " << s;

    emit readyRead(bytes);//сигнал прочитанной строкой
}

void GPS::ubxParser(const QByteArray &bytes)
{
    if(bytes.isEmpty()) {
        return;
    }
    //парсим здесь
    //потом currentCoordinate.setLatitude(...); ...



//    latLonToXY(currentCoordinate.latitude(), currentCoordinate.longitude());

//    qDebug() << "coord.latitude(): " << QString::number(currentCoordinate.latitude(), 'g', 9);
//    qDebug() << "coord.longitude(): " << QString::number(currentCoordinate.longitude(), 'g', 9);

//    double lat = currentCoordinate.latitude();
//    double lon = currentCoordinate.longitude();

//    emit updatePositionXY(this->x, this->y);
//    emit updatePositionLatLon(lat, lon);
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
