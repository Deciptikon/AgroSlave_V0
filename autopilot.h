#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include <QObject>
#include <QDebug>
#include <QtMath>
#include <QVector3D>

class Autopilot : public QObject
{
    Q_OBJECT
public:
    explicit Autopilot(QObject *parent = nullptr);

    void init(const int secDeltaTime);

    int getMSecDeltaTime() const;

    void restateRelay();

signals:
    //void sendCommandToSlave14(int &comm);

public slots:
    void loop();

    void readFromGPS(const double &x, const double &y);

private slots:

private:
    int msecDeltaTime;

    bool stateRelay; //состояние реле подруливающего устройства
};

#endif // AUTOPILOT_H
