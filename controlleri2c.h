#ifndef CONTROLLERI2C_H
#define CONTROLLERI2C_H

#include <QObject>
#include <QDebug>

#include <wiringPi.h>
#include <wiringPiI2C.h>

class ControllerI2C : public QObject
{
    Q_OBJECT
public:
    explicit ControllerI2C(QObject *parent = nullptr);

    void init(int hexAdress);

signals:
    void readFrom(int &data);

public slots:
    void readData();
    void writeData(const int &data);

private:
    int hexAdress;

    int deviceRegAdress;

};

#endif // CONTROLLERI2C_H
