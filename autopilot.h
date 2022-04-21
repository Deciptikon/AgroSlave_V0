#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include <QObject>
#include <QDebug>
#include <QtMath>
#include <QList>
#include <QVector2D>
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
    void sendCommandToSlave14(int &comm);

public slots:
    void loop();

    void readFromGPS(const double &x, const double &y);

    void createListPoint();

private slots:

private:
    int msecDeltaTime;

    bool stateRelay; //состояние реле подруливающего устройства

    bool isOrigin = false;
    double xOrigin = 0;//локальныая система координат
    double yOrigin = 0;

    QVector2D direction;//направление перемещения, единичный вектор
    QVector2D directionToPoint;

    QList<QVector2D> path2D;

    QList<QVector2D> listPoint2D;
};

#endif // AUTOPILOT_H
