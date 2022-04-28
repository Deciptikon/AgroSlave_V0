#include "viewdata.h"

ViewData::ViewData(QObject *parent) : QObject(parent)
{

}

double ViewData::xCord() const
{
    return m_xCord;
}

void ViewData::setXCord(double newXCord)
{
    if (qFuzzyCompare(m_xCord, newXCord))
        return;
    m_xCord = newXCord;
    emit xCordChanged();
    emit xCordChanged(m_xCord);
}

double ViewData::yCord() const
{
    return m_yCord;
}

void ViewData::setYCord(double newYCord)
{
    if (qFuzzyCompare(m_yCord, newYCord))
        return;
    m_yCord = newYCord;
    emit yCordChanged();
    emit yCordChanged(m_yCord);
}

void ViewData::acceptCoord(const double &x, const double &y)
{
    setXCord(x);
    setYCord(y);
}

void ViewData::slotCommandToSlave14(int comm)
{
    emit signalCommandToSlave14(comm);
}

void ViewData::acceptPath(const ListVector &path)
{
    this->path = path;
    emit pathToQML(this->path);
}

void ViewData::acceptKeyPoints(const ListVector &keyPoints)
{
    this->keyPoints = keyPoints;
    emit keyPointsToQML(this->keyPoints);
}

void ViewData::addKeyPointFromQML(const QVector2D point)
{
    emit sendKeyPointForAdding(point);
}

void ViewData::slotCreateQuadroKeyPoint()
{
    emit signalCreateQuadroKeyPoint();
}
