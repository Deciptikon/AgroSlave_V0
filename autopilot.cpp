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
    //here calculate drive


}

void Autopilot::readFromGPS(const double &x, const double &y)
{
    qDebug() << "Autopilot::readFromGPS(double &x, double &y)" << x << "\t |" << y;

}

void Autopilot::createListPoint()
{
    //f(direct)
    listPoint2D.append({1,2});
    listPoint2D.append({3,4});
    listPoint2D.append({5,6});
    listPoint2D.append({7,8});
    listPoint2D.append({9,10});
}

int Autopilot::getMSecDeltaTime() const
{
    return msecDeltaTime;
}

void Autopilot::restateRelay()
{
    stateRelay = !stateRelay;
    qDebug() << "Состояние реле изменено на " << stateRelay;

    int c='c';
    emit sendCommandToSlave14(c);
}
