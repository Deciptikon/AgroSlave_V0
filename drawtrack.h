#ifndef DRAWTRACK_H
#define DRAWTRACK_H

#include <QQuickPaintedItem>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QPainterPath>

typedef QList<QVector2D> ListVector;

class DrawTrack : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(qreal msecUpdate READ msecUpdate WRITE setMsecUpdate NOTIFY msecUpdateChanged)
    Q_PROPERTY(ListVector path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(ListVector keypoint READ keypoint WRITE setKeypoint NOTIFY keypointChanged)
    Q_PROPERTY(QVector2D originPoint READ originPoint WRITE setOriginPoint NOTIFY originPointChanged)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(qreal widthPath READ widthPath WRITE setWidthPath NOTIFY widthPathChanged)
    Q_PROPERTY(QColor colorPath READ colorPath WRITE setColorPath NOTIFY colorPathChanged)
    Q_PROPERTY(QVector2D shiftCord READ shiftCord WRITE setShiftCord NOTIFY shiftCordChanged)
    Q_PROPERTY(bool isCenteredLastPoint READ isCenteredLastPoint WRITE setIsCenteredLastPoint NOTIFY isCenteredLastPointChanged)
    Q_PROPERTY(bool isPaintAxis READ isPaintAxis WRITE setIsPaintAxis NOTIFY isPaintAxisChanged)

public:
    explicit DrawTrack(QQuickItem *parent = 0);

    void paint(QPainter *painter) override;


    Q_INVOKABLE void swapCentered();
    Q_INVOKABLE void updatePath(const ListVector path);
    Q_INVOKABLE void appPointToPath(const QVector2D vec);
    Q_INVOKABLE void appPointToPathAndRemoveFirst(const QVector2D vec);
    Q_INVOKABLE void updateKeyPoint(const ListVector points);
    Q_INVOKABLE void zoomIn();
    Q_INVOKABLE void zoomOut();
    Q_INVOKABLE QString getZoom();


    const ListVector &path() const;
    void setPath(const ListVector &newPath);

    const ListVector &keypoint() const;
    void setKeypoint(const ListVector &newKeypoint);

    const QVector2D &originPoint() const;
    void setOriginPoint(const QVector2D &newOriginPoint);

    qreal zoom() const;
    void setZoom(qreal newZoom);

    qreal widthPath() const;
    void setWidthPath(qreal newWidthPath);

    const QColor &colorPath() const;
    void setColorPath(const QColor &newColorPath);

    bool isCenteredLastPoint() const;
    void setIsCenteredLastPoint(bool newIsCenteredLastPoint);

    qreal msecUpdate() const;
    void setMsecUpdate(qreal newMsecUpdate);

    bool isPaintAxis() const;
    void setIsPaintAxis(bool newIsPaintAxis);

    const QVector2D &shiftCord() const;
    void setShiftCord(const QVector2D &newShiftCord);

protected: // QQuickItem interface
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void pathChanged();

    void keypointChanged();

    void originPointChanged();

    void zoomChanged();

    void widthPathChanged();

    void colorPathChanged();

    void isCenteredLastPointChanged();

    void msecUpdateChanged();

    void isPaintAxisChanged();

    void releaseCoordinate(const QVector2D &coord);

    void shiftCordChanged();

private:
    void drawAxis(QPainter *painter);
    void drawPath(QPainter *painter);
    void drawKeypoint(QPainter *painter);
    void drawMouseEvent(QPainter *painter);//рисование области нажатия на дисплее

    void pathToPaintedPath();

    QTimer      *internalTimer;

    ListVector m_path;//точки пути
    ListVector m_keypoint;//точки ключевые

    // от этой точки будут вестись все расчеты
    // это локальное начало координат
    QVector2D m_originPoint;

    QPainterPath pathForDraw;//путь отрисовываемый

    qreal m_zoom;
    qreal m_widthPath;
    QColor m_colorPath;

    // центрировать ли на текущем положении
    // или на начале координат
    bool m_isCenteredLastPoint;

    qreal m_msecUpdate;//интевал обновления отрисовки

    bool m_isPaintAxis;//рисовать ли оси

    // события нажатия на экран мышью/пальцем
    bool isPressed = false;
    QPointF mouseEvent;

    // смещение центра экрана от начала координат
    // равно нулю при центрировании на начале координат
    // не равно нулю при центрировании на текущем положении
    QVector2D m_shiftCord;
};

#endif // DRAWTRACK_H
