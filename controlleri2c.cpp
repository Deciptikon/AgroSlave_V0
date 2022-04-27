#include "controlleri2c.h"

ControllerI2C::ControllerI2C(QObject *parent) : QObject(parent)
{

}

void ControllerI2C::init(int hexAdress)
{
    if(hexAdress>0x08 && hexAdress<0x80) {//0-128
        this->hexAdress = hexAdress;
    } else {
        this->hexAdress = 0;
    }

    this->deviceRegAdress = wiringPiI2CSetup(this->hexAdress);
    qDebug() << "Setup deviceRegAdress N" << this->hexAdress << "= " << this->deviceRegAdress;
}

//чтение по команде
void ControllerI2C::readData()
{
//    if (this->deviceRegAdress == -1) {
//        qDebug() << "[SlaveController::loop()] deviceRegAdress == -1";
//    } else {
//        uint8_t g = 'g';
//        uint8_t f = 'f';

//        int received_data  = wiringPiI2CReadReg8(this->deviceRegAdress, g);
//        int received_data2 = wiringPiI2CReadReg8(this->deviceRegAdress, f);

//        int rd = received_data2*100 + received_data;
//        emit readFrom(rd);

//        qDebug() << "-----------------------------------------------";
//        qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read 1: " << received_data;
//        qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read 2: " << received_data2;
//        qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read summ: " << rd;
//    }
}

void ControllerI2C::writeData(const int &data)
{
    if (this->deviceRegAdress == -1) {
        qDebug() << "[SlaveController::loop()] deviceRegAdress == -1";
    } else {
        wiringPiI2CWrite(this->deviceRegAdress, data);
        qDebug() << "SlaveController::writeData()";
    }
}
