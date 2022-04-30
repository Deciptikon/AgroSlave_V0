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

    Q_PROPERTY(qreal  msecUpdate  READ msecUpdate  WRITE setMsecUpdate  NOTIFY msecUpdateChanged)
    Q_PROPERTY(qreal  zoom        READ zoom        WRITE setZoom        NOTIFY zoomChanged)
    Q_PROPERTY(qreal  widthPath   READ widthPath   WRITE setWidthPath   NOTIFY widthPathChanged)
    Q_PROPERTY(QColor colorPath   READ colorPath   WRITE setColorPath   NOTIFY colorPathChanged)
    Q_PROPERTY(QColor colorKeyPoint READ colorKeyPoint WRITE setColorKeyPoint NOTIFY colorKeyPointChanged)
    Q_PROPERTY(QColor colorGround READ colorGround WRITE setColorGround NOTIFY colorGroundChanged)
    Q_PROPERTY(bool   isPaintAxis READ isPaintAxis WRITE setIsPaintAxis NOTIFY isPaintAxisChanged)
    Q_PROPERTY(bool   isCenteredLastPoint READ isCenteredLastPoint WRITE setIsCenteredLastPoint NOTIFY isCenteredLastPointChanged)
    Q_PROPERTY(bool   isUpdateFromChanged READ isUpdateFromChanged WRITE setIsUpdateFromChanged NOTIFY isUpdateFromChangedChanged)

public:
    explicit DrawTrack(QQuickItem *parent = 0);

    void paint(QPainter *painter) override;


    Q_INVOKABLE void swapCentered();
    Q_INVOKABLE void appPointToPath(const QVector2D vec);
    Q_INVOKABLE void appPointToPathAndRemoveFirst(const QVector2D vec);
    Q_INVOKABLE void updateKeyPoint(const ListVector points);
    Q_INVOKABLE void zoomIn();
    Q_INVOKABLE void zoomOut();
    Q_INVOKABLE QString getZoom();


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

    bool isUpdateFromChanged() const;
    void setIsUpdateFromChanged(bool newIsUpdateFromChanged);

    const QColor &colorGround() const;
    void setColorGround(const QColor &newColorGround);

    const QColor &colorKeyPoint() const;
    void setColorKeyPoint(const QColor &newColorKeyPoint);

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

    void isUpdateFromChangedChanged();

    void colorGroundChanged();

    void colorKeyPointChanged();

private slots:
    void intervalChanged();
    void checkAndUpdate();

private:
    void drawAxis(QPainter *painter);
    void drawPath(QPainter *painter);
    void drawKeypoint(QPainter *painter);
    void drawMouseEvent(QPainter *painter);//рисование области нажатия на дисплее

    void pathToPaintedPath();

    QTimer      *internalTimer;

    ListVector m_path;// точки пути
    ListVector m_keypoint;// точки ключевые

    // от этой точки будут вестись все расчеты
    // это локальное начало координат
    QVector2D m_originPoint;

    QPainterPath pathForDraw;// путь отрисовываемый

    qreal m_zoom;// приближение

    qreal m_widthPath;// ширина пути
    QColor m_colorPath;// цвет пути

    // центрировать ли на текущем положении
    // или на начале координат
    bool m_isCenteredLastPoint;

    qreal m_msecUpdate;// интевал обновления отрисовки

    bool m_isPaintAxis;// рисовать ли оси

    // события нажатия на экран мышью/пальцем
    bool isPressed = false;
    QPointF mouseEvent;

    // смещение центра экрана от начала координат
    // равно нулю при центрировании на начале координат
    // не равно нулю при центрировании на текущем положении
    QVector2D m_shiftCord;

    // обновление при изменении пути или клюяевых точек
    bool m_isUpdateFromChanged;

    QColor m_colorGround;// цвет фона

    QColor m_colorKeyPoint;// цвет ключевых точек
};

#endif // DRAWTRACK_H
