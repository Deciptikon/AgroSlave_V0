#include "autopilot.h"


Autopilot::Autopilot(QObject *parent) : QObject(parent)
{

}

void Autopilot::init(const int msecDeltaTime)
{
    if(msecDeltaTime > 1) {
        this->msecDeltaTime = msecDeltaTime;
    } else {
        this->msecDeltaTime = 1;
    }

}

void Autopilot::loop()
{
    qDebug() << "void Autopilot::loop()";

    //update velocity and translate
    //

}

void Autopilot::readFromGPS(const double &x, const double &y)
{
    qDebug() << "Autopilot::readFromGPS(double &x, double &y)";
}

int Autopilot::getMSecDeltaTime() const
{
    return msecDeltaTime;
}

void Autopilot::restateRelay()
{
    stateRelay = !stateRelay;
    qDebug() << "Состояние реле изменено на " << stateRelay;

    //int c='c';
    //emit sendCommandToSlave14(c);
}
