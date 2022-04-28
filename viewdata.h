#ifndef VIEWDATA_H
#define VIEWDATA_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QVector2D>
#include <QDebug>

typedef QList<QVector2D> ListVector;

class ViewData : public QObject
{
    Q_OBJECT

public:
    explicit ViewData(QObject *parent = nullptr);

    Q_PROPERTY(double xCord READ xCord WRITE setXCord NOTIFY xCordChanged)
    Q_PROPERTY(double yCord READ yCord WRITE setYCord NOTIFY yCordChanged)

    double xCord() const;
    void setXCord(double newXCord);

    double yCord() const;
    void setYCord(double newYCord);

signals:

    void xCordChanged();
    void xCordChanged(double x);

    void yCordChanged();
    void yCordChanged(double y);

    void signalCommandToSlave14(const int &comm);

    void pathToQML(const ListVector path);

    void signalAppPointToPathQML(const QVector2D vec);

    void signalAppPointToPathAndRemoveFirstQML(const QVector2D vec);

    void keyPointsToQML(const ListVector keyPoints);

    void sendKeyPointForAdding(const QVector2D &point);

    void signalCreateQuadroKeyPoint();

public slots:
    void acceptCoord(const double &x, const double &y);

    void slotCommandToSlave14(int comm);

    void acceptPath(const ListVector &path);

    void slotAppPointToPath(const QVector2D &vec);

    void slotAppPointToPathAndRemoveFirst(const QVector2D &vec);

    void acceptKeyPoints(const ListVector &keyPoints);

    void addKeyPointFromQML(const QVector2D point);

    void slotCreateQuadroKeyPoint();

private:
    double m_xCord;
    double m_yCord;

    ListVector path;
    ListVector keyPoints;
};

#endif // VIEWDATA_H
