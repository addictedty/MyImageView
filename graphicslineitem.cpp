#include "graphicslineitem.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

GraphicsLineItem::GraphicsLineItem(QGraphicsLineItem* parent) : QGraphicsLineItem(parent)
{
    m_pen = pen();
    m_pen.setColor(Qt::red);
    m_pen.setWidth(4);
    setPen(m_pen);

    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    m_isDrawing = true;
}

GraphicsLineItem::GraphicsLineItem(QLineF linef)
{
    m_pen = pen();
    m_pen.setColor(Qt::red);
    m_pen.setWidth(4);
    setPen(m_pen);

    setLine(linef);
    setEnabled(false);
    setZValue(-1);
}

QVector<QRectF> GraphicsLineItem::anchorItem(const QLineF &linef)
{
    QVector<QRectF> anchor;

    qreal halfpw = linef.length() / 10;

    if(linef.p1().x() <= linef.p2().x())
    {
        if (linef.y1() <= linef.y2())
        {
            anchor.append(QRectF(linef.x1(),linef.y1(),halfpw,halfpw));
            anchor.append(QRectF(linef.x2() - halfpw,linef.y2() - halfpw,halfpw,halfpw));
        }
        else
        {
            anchor.append(QRectF(linef.x1(),linef.y1() - halfpw,halfpw,halfpw));
            anchor.append(QRectF(linef.x2() - halfpw,linef.y2(),halfpw,halfpw));
        }
    }
    else
    {
        if (linef.y1() <= linef.y2())
        {
            anchor.append(QRectF(linef.x1() - halfpw,linef.y1(),halfpw,halfpw));
            anchor.append(QRectF(linef.x2(),linef.y2() - halfpw,halfpw,halfpw));
        }
        else
        {
            anchor.append(QRectF(linef.x1() - halfpw,linef.y1() - halfpw,halfpw,halfpw));
            anchor.append(QRectF(linef.x2(),linef.y2(),halfpw,halfpw));
        }
    }

    return anchor;
}

void GraphicsLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
    QVector<QRectF> set = anchorItem(line());

    do
    {
        if(set[0].contains(pos))
        {
            m_edge = Edge::Top;
            break;
        }
        if(set[1].contains(pos))
        {
            m_edge = Edge::Bottom;
            break;
        }
        m_edge = Edge::Center;

    }while(false);

    setFlag(QGraphicsItem::ItemIsMovable, false);

    QGraphicsLineItem::mousePressEvent(event);
}

void GraphicsLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);

    QGraphicsLineItem::mouseReleaseEvent(event);
}

void GraphicsLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->pos();
    QLineF linef = line();

    switch (m_edge) {
    case Edge::Top:
        linef.setP1(pos);
        setLine(linef);
        break;
    case Edge::Bottom:
        linef.setP2(pos);
        setLine(linef);
        break;
    case Edge::Center:
        setFlag(QGraphicsItem::ItemIsMovable, true);
        break;
    default:
        break;
    }

    QGraphicsLineItem::mouseMoveEvent(event);
}

void GraphicsLineItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos = event->pos();
    QVector<QRectF> set = anchorItem(line());

    if(set[0].contains(pos))
        setCursor(Qt::SizeVerCursor);
    else if(set[1].contains(pos))
        setCursor(Qt::SizeVerCursor);
    else
        setCursor(Qt::SizeAllCursor);

    QGraphicsLineItem::hoverMoveEvent(event);
}

void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    m_line = line();
    QFont font = painter->font();
    font.setPointSize(40);
    painter->setFont(font);
    if (option->state & QStyle::State_Selected)
    {
        QVector<QRectF> set = anchorItem(m_line);
        m_pen.setColor(Qt::green);
        painter->setPen(m_pen);
        for(int i = 0; i < set.count(); i++)
        {
            painter->drawRect(set[i]);
        }

        QPointF p1 = mapToScene(m_line.p1());
        QPointF p2 = mapToScene(m_line.p2());
        QString strP1 = QString::asprintf("X1:%.2f\r\nY1:%.2f", p1.x(), p1.y());
        QString strP2 = QString::asprintf("X2:%.2f\r\nY2:%.2f", p2.x(), p2.y());
        if(m_line.p1().x() <= m_line.p2().x())
        {
            if(m_line.p1().y() >= m_line.p2().y())
            {
                painter->drawText(QRectF(m_line.p1().x() - 400, m_line.p1().y() - 165, 400, 165), strP1);
                painter->drawText(QRectF(m_line.p2().x(), m_line.p2().y(), 400, 165), strP2);
            }
            else
            {
                painter->drawText(QRectF(m_line.p1().x() - 400, m_line.p1().y(), 400, 165), strP1);
                painter->drawText(QRectF(m_line.p2().x(), m_line.p2().y() - 165, 400, 165), strP2);
            }
        }
        else
        {
            if(m_line.p1().y() >= m_line.p2().y())
            {
                painter->drawText(QRectF(m_line.p1().x(), m_line.p1().y() - 165, 400, 165), strP1);
                painter->drawText(QRectF(m_line.p2().x() - 400, m_line.p2().y(), 400, 165), strP2);
            }
            else
            {
                painter->drawText(QRectF(m_line.p1().x(), m_line.p1().y(), 400, 165), strP1);
                painter->drawText(QRectF(m_line.p2().x() - 400, m_line.p2().y() - 165, 400, 165), strP2);
            }

        }
        QString lineLength = QString::asprintf("Line Length: %.2f\r\nLine Angle: %.2f", m_line.length(), m_line.angle());
        painter->drawText(QRectF(mapFromScene(40, 40).x(), mapFromScene(40 ,40).y(), 800, 165), lineLength);

        m_pen.setColor(Qt::red);
    }

    QGraphicsLineItem::paint(painter, option, widget);
}
