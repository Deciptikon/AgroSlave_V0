#include "drawtrack.h"

DrawTrack::DrawTrack(QQuickItem *parent):
    m_originPoint(0,0),
    m_zoom(2),
    m_widthPath(2),
    m_colorPath(255,0,0),
    m_isCenteredLastPoint(false),
    m_msecUpdate(50),
    m_isPaintAxis(true),
    m_shiftCord(0,0)
{
    internalTimer = new QTimer(this);
    connect(internalTimer, &QTimer::timeout, [=](){
        count++;
        update();
    } );
    internalTimer->start(m_msecUpdate);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void DrawTrack::paint(QPainter *painter)
{
    drawAxis(painter);
    drawPath(painter);
    drawKeypoint(painter);
    drawMouseEvent(painter);
}

void DrawTrack::swapCentered()
{
    setIsCenteredLastPoint(!isCenteredLastPoint());
}

void DrawTrack::updatePath(const ListVector path)
{
    setPath(path);
}

void DrawTrack::updateKeyPoint(const ListVector points)
{
    qDebug() << "keypoints ---> " << m_keypoint;
    setKeypoint(points);
}

void DrawTrack::drawAxis(QPainter *painter)
{
    if(!m_isPaintAxis) {
        return;
    }
    if(painter == nullptr) {
        return;
    }
    if(!painter->isActive()) {
        return;
    }

    float marginAxis=5;

    QPen penLine{Qt::NoBrush, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    penLine.setColor(QColor(0,0,0));//черные оси

    painter->save();
    painter->translate( this->width()/2, this->height()/2 );

    painter->setPen(penLine);

    painter->drawLine(QPoint(marginAxis - this->width()/2 , 0 ),//X
                      QPoint(this->width()/2 - marginAxis , 0));

    painter->drawLine(QPoint(0 , marginAxis - this->height()/2 ),//Y
                      QPoint(0 , this->height()/2 - marginAxis));

    QPointF n = QPointF(0, 0);

    auto paintEllipse = [&](qreal radius) {
        painter->drawEllipse(n, radius*m_zoom, radius*m_zoom);
        painter->drawText(QPointF{radius*m_zoom + 1, -5}, QString::number(radius));
    };

    paintEllipse(10);
    paintEllipse(25);
    paintEllipse(50);
    paintEllipse(100);
    paintEllipse(200);
    paintEllipse(400);


    //painter->drawText(QPointF{55,-25}, QString::number(count));/////////

    painter->restore();
}

void DrawTrack::drawPath(QPainter *painter)
{
    //qDebug() << "path.length" << pathForDraw.length();

    if(pathForDraw.isEmpty()) {
        return;
    }
    if(painter == nullptr) {
        return;
    }
    if(!painter->isActive()) {
        return;
    }

    QPen penLine{Qt::NoBrush, m_widthPath/m_zoom, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    penLine.setColor(m_colorPath);//
    painter->save();
    painter->setPen(penLine);

    painter->translate( this->width()/2 - m_shiftCord.x()*m_zoom,
                        this->height()/2 - m_shiftCord.y()*m_zoom);

    painter->scale(m_zoom, m_zoom);

    painter->drawPath(pathForDraw);

    painter->restore();
}

void DrawTrack::drawKeypoint(QPainter *painter)
{
    if(m_keypoint.isEmpty()) {
        return;
    }
    if(painter == nullptr) {
        return;
    }
    if(!painter->isActive()) {
        return;
    }

    QPen penLine{Qt::NoBrush, 2.0/m_zoom, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    penLine.setColor(QColor(0,255,0));
    painter->setPen(penLine);
    painter->save();
    painter->translate( this->width()/2 - m_shiftCord.x()*m_zoom,
                        this->height()/2 - m_shiftCord.y()*m_zoom);

    painter->scale(m_zoom, m_zoom);

    float r = 10;
    for (auto p: qAsConst(m_keypoint)) {
        painter->drawEllipse(p.toPointF(), r, r);
    }

    painter->restore();
}

void DrawTrack::drawMouseEvent(QPainter *painter)
{
    if(!isPressed) {
        return;
    }
    if(painter == nullptr) {
        return;
    }
    if(!painter->isActive()) {
        return;
    }

//    QPen penEllipse;
//    penEllipse.setColor(QColor(0,255,0));
//    penEllipse.setWidth(2);
    QPen penLine{Qt::NoBrush, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    penLine.setColor(QColor(255,0,255));
    painter->save();
    painter->setPen(penLine);

    float r = 20;
    painter->drawEllipse(mouseEvent, r, r);

    painter->restore();
}

const QVector2D DrawTrack::getShiftCord() const
{
    return m_shiftCord;
}

const ListVector &DrawTrack::path() const
{
    return m_path;
}

void DrawTrack::setPath(const ListVector &newPath)
{
    if (m_path == newPath)
        return;

    m_path = newPath;

    pathForDraw.clear();

    if(m_path.isEmpty()) {
        return;
    }

    if(m_isCenteredLastPoint) {
        setShiftCord(m_path.last() - originPoint());
    } else {
        setShiftCord({0,0});
    }

    pathForDraw.moveTo((m_path.first() - originPoint()).toPointF());
    for(auto p: m_path) {
        pathForDraw.lineTo((p - originPoint()).toPointF());

    }

    emit pathChanged();
}

const ListVector &DrawTrack::keypoint() const
{
    return m_keypoint;
}

void DrawTrack::setKeypoint(const ListVector &newKeypoint)
{
    if (m_keypoint == newKeypoint)
        return;
    m_keypoint = newKeypoint;
    emit keypointChanged();
}

const QVector2D &DrawTrack::originPoint() const
{
    return m_originPoint;
}

void DrawTrack::setOriginPoint(const QVector2D &newOriginPoint)
{
    if (m_originPoint == newOriginPoint)
        return;
    m_originPoint = newOriginPoint;
    emit originPointChanged();
}

qreal DrawTrack::zoom() const
{
    return m_zoom;
}

void DrawTrack::setZoom(qreal newZoom)
{
    if (qFuzzyCompare(0, newZoom))
        return;
    if (qFuzzyCompare(m_zoom, newZoom))
        return;
    if(newZoom > 100)
        return;
    if(newZoom < 0.01)
        return;
    m_zoom = newZoom;
    emit zoomChanged();
}

void DrawTrack::zoomIn()
{
    setZoom(zoom()*2);
}

void DrawTrack::zoomOut()
{
    setZoom(zoom()*0.5);
}

QString DrawTrack::getZoom()
{
    return QString::number(zoom(),'g',3);
}

qreal DrawTrack::widthPath() const
{
    return m_widthPath;
}

void DrawTrack::setWidthPath(qreal newWidthPath)
{
    if (qFuzzyCompare(m_widthPath, newWidthPath))
        return;
    m_widthPath = newWidthPath;
    emit widthPathChanged();
}

const QColor &DrawTrack::colorPath() const
{
    return m_colorPath;
}

void DrawTrack::setColorPath(const QColor &newColorPath)
{
    if (m_colorPath == newColorPath)
        return;
    m_colorPath = newColorPath;
    emit colorPathChanged();
}

bool DrawTrack::isCenteredLastPoint() const
{
    return m_isCenteredLastPoint;
}

void DrawTrack::setIsCenteredLastPoint(bool newIsCenteredLastPoint)
{
    if (m_isCenteredLastPoint == newIsCenteredLastPoint)
        return;
    m_isCenteredLastPoint = newIsCenteredLastPoint;
    emit isCenteredLastPointChanged();
}

qreal DrawTrack::msecUpdate() const
{
    return m_msecUpdate;
}

void DrawTrack::setMsecUpdate(qreal newMsecUpdate)
{
    if (qFuzzyCompare(m_msecUpdate, newMsecUpdate))
        return;
    m_msecUpdate = newMsecUpdate;
    emit msecUpdateChanged();
}

bool DrawTrack::isPaintAxis() const
{
    return m_isPaintAxis;
}

void DrawTrack::setIsPaintAxis(bool newIsPaintAxis)
{
    if (m_isPaintAxis == newIsPaintAxis)
        return;
    m_isPaintAxis = newIsPaintAxis;
    emit isPaintAxisChanged();
}

void DrawTrack::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        isPressed = true;
        mouseEvent = event->localPos();
        update();
    }
}

void DrawTrack::mouseMoveEvent(QMouseEvent *event)
{
    if(isPressed) {
        mouseEvent = event->localPos();
        update();
    }
}

void DrawTrack::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        qDebug() <<" coord:" << event->pos();
        isPressed = false;
        mouseEvent = event->localPos();
        update();
        emit releaseCoordinate(mouseEvent);
    }
}

const QVector2D &DrawTrack::shiftCord() const
{
    return m_shiftCord;
}

void DrawTrack::setShiftCord(const QVector2D &newShiftCord)
{
    if (m_shiftCord == newShiftCord)
        return;
    m_shiftCord = newShiftCord;
    emit shiftCordChanged();
}
