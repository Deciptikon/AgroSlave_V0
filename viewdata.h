#ifndef VIEWDATA_H
#define VIEWDATA_H

#include <QObject>

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

public slots:
    void acceptCoord(const double &x, const double &y);

private:
    double m_xCord;
    double m_yCord;
};

#endif // VIEWDATA_H
