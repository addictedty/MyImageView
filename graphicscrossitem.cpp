#include "graphicscrossitem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

GraphicsCrossItem::GraphicsCrossItem(QGraphicsItem* parent) : QGraphicsItem(parent)
{
    m_pen.setColor(Qt::red);
    m_pen.setWidth(4);

    setFlag(QGraphicsItem::ItemIsSelectable);

    setAcceptHoverEvents(true);
}

GraphicsCrossItem::GraphicsCrossItem(QPointF center, qreal width, qreal height)
{
    m_pen.setColor(Qt::red);
    m_pen.setWidth(4);

    m_rect.setRect(center.x() - width / 2, center.y() - height / 2, width, height);

    setEnabled(false);
    setZValue(-1);
}

void GraphicsCrossItem::setCross(const QPointF &center, qreal width, qreal height)
{
    QRectF rectf(center.x() - width / 2, center.y() - height / 2, width, height);
    m_rect = mapRectFromScene(rectf);
}

QRectF GraphicsCrossItem::getRect()
{
    if(m_rect != QRectF())
        return m_rect;
    return QRectF();
}

QVector<QRectF> GraphicsCrossItem::anchorItem(const QRectF &rectf)
{
    QVector<QRectF> anchor;

    qreal width = rectf.width() / 10;
    qreal height = rectf.height() / 10;

    anchor.append(QRectF(rectf.topLeft().x(), rectf.topLeft().y(),
                  width, height));
    anchor.append(QRectF(rectf.topRight().x() - width, rectf.topRight().y(),
                  width, height));
    anchor.append(QRectF(rectf.bottomLeft().x(), rectf.bottomLeft().y() - height,
                  width, height));
    anchor.append(QRectF(rectf.bottomRight().x() - width, rectf.bottomRight().y() - height,
                  width, height));

    return anchor;
}

void GraphicsCrossItem::adjustRect()
{
    qreal left = m_rect.left();
    qreal right = m_rect.right();
    qreal top = m_rect.top();
    qreal bottom = m_rect.bottom();
    m_rect.setLeft(qMin(left, right));
    m_rect.setRight(qMax(left, right));
    m_rect.setTop(qMin(top, bottom));
    m_rect.setBottom(qMax(top, bottom));
}

QRectF GraphicsCrossItem::boundingRect() const
{
    if(m_rect != QRectF())
        return m_rect;
    return QRectF();
}

void GraphicsCrossItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        m_isDrawing = false;
        setEnabled(false);
        setVisible(false);
        setCursor(Qt::ArrowCursor);
        emit drawFinished();
        return;
    }

    QPointF pos = event->pos();
    QVector<QRectF> set = anchorItem(m_rect);
    do
    {
        if(set[0].contains(pos))
        {
            m_edge = Edge::TopLeft;
            break;
        }
        if(set[1].contains(pos))
        {
            m_edge = Edge::TopRight;
            break;
        }
        if(set[2].contains(pos))
        {
            m_edge = Edge::BottomLeft;
            break;
        }
        if(set[3].contains(pos))
        {
            m_edge = Edge::BottomRight;
            break;
        }
        m_edge = Edge::Center;

    }while(false);

    m_rectCenter = m_rect.center();

    setFlag(QGraphicsItem::ItemIsMovable, false);

    QGraphicsItem::mousePressEvent(event);
}

void GraphicsCrossItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->pos();

    switch (m_edge) {
    case Edge::TopLeft:
        m_rect.setTopLeft(pos);
        m_rect.setBottomRight(2 * m_rectCenter - m_rect.topLeft());
        break;
    case Edge::TopRight:
        m_rect.setTopRight(pos);
        m_rect.setBottomLeft(2 * m_rectCenter - m_rect.topRight());
        break;
    case Edge::BottomLeft:
        m_rect.setBottomLeft(pos);
        m_rect.setTopRight(2 * m_rectCenter - m_rect.bottomLeft());
        break;
    case Edge::BottomRight:
        m_rect.setBottomRight(pos);
        m_rect.setTopLeft(2 * m_rectCenter - m_rect.bottomRight());
        break;
    case Edge::Center:
        setFlag(QGraphicsItem::ItemIsMovable, true);
        break;
    default:
        break;
    }

    prepareGeometryChange();

    QGraphicsItem::mouseMoveEvent(event);
}

void GraphicsCrossItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        switch (m_edge)
        {
        case Edge::TopLeft:
        case Edge::TopRight:
        case Edge::BottomLeft:
        case Edge::BottomRight:
            adjustRect();
            break;
        case Edge::Center:
            setFlag(QGraphicsItem::ItemIsMovable, true);
            break;
        default:
            break;
        }

        prepareGeometryChange();
    }

    setFlag(QGraphicsItem::ItemIsMovable, true);

    QGraphicsItem::mouseReleaseEvent(event);
}

void GraphicsCrossItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos = event->pos();
    QVector<QRectF> set = anchorItem(m_rect);

    if(set[0].contains(pos))
        setCursor(Qt::SizeFDiagCursor);
    else if(set[1].contains(pos))
        setCursor(Qt::SizeBDiagCursor);
    else if(set[2].contains(pos))
        setCursor(Qt::SizeBDiagCursor);
    else if(set[3].contains(pos))
        setCursor(Qt::SizeFDiagCursor);
    else
        setCursor(Qt::SizeAllCursor);

    QGraphicsItem::hoverMoveEvent(event);
}

void GraphicsCrossItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->setPen(m_pen);

    QLineF line1(m_rect.center().x(), m_rect.top(), m_rect.center().x(), m_rect.bottom());
    QLineF line2(m_rect.left(), m_rect.center().y(), m_rect.right(), m_rect.center().y());
    painter->drawLine(line1);
    painter->drawLine(line2);

    if (option->state & QStyle::State_Selected)
    {
        QPen pen = painter->pen();
        pen.setColor(Qt::green);
        pen.setWidth(5);
        painter->setPen(pen);
        QVector<QRectF> set = anchorItem(m_rect);
        for(int i = 0; i < set.count(); i++)
        {
            painter->drawRect(set[i]);
        }

        pen.setStyle(Qt::DotLine);
        pen.setColor(Qt::white);
        painter->setPen(pen);
        painter->drawRect(m_rect);
    }
}
