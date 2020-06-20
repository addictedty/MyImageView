#include "graphicsrectitem.h"
#include <QPen>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDebug>

GraphicsRectItem::GraphicsRectItem(QGraphicsRectItem* parent) : QGraphicsRectItem(parent)
{
    m_pen.setColor(Qt::red);
    m_pen.setWidth(4);
    setPen(m_pen);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    m_isDrawing = true;
}

GraphicsRectItem::GraphicsRectItem(QRectF rectf)
{
    m_pen.setColor(Qt::red);
    m_pen.setWidth(4);
    setPen(m_pen);

    setRect(rectf);
    setEnabled(false);
    setZValue(-1);
}

void GraphicsRectItem::showCross(bool flag)
{
    m_showCross = flag;
}

QVector<QRectF> GraphicsRectItem::anchorItem(const QRectF &rectf)
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

void GraphicsRectItem::adjustRect()
{
    QRectF rectf = rect();

    qreal left = rectf.left();
    qreal right = rectf.right();
    qreal top = rectf.top();
    qreal bottom = rectf.bottom();

    rectf.setLeft(qMin(left, right));
    rectf.setRight(qMax(left, right));
    rectf.setTop(qMin(top, bottom));
    rectf.setBottom(qMax(top, bottom));

    qDebug() << "left: = " << left << "right: = " << right << "top: = " << top << "bottom: = " << bottom;

    setRect(rectf);
}

void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    m_rect = rect();
    if (option->state & QStyle::State_Selected)
    {
        QVector<QRectF> set = anchorItem(m_rect);
        m_pen.setColor(Qt::green);
        painter->setPen(m_pen);
        for(int i = 0; i < set.count(); i++)
        {
            painter->drawRect(set[i]);
        }

        if(m_showCross)
        {
            painter->drawLine(m_rect.left(), m_rect.center().y(), m_rect.right(), m_rect.center().y());
            painter->drawLine(m_rect.center().x(), m_rect.top(), m_rect.center().x(), m_rect.bottom());
        }

        m_pen.setColor(Qt::red);
    }

    QGraphicsRectItem::paint(painter, option, widget);
}

void GraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
    QVector<QRectF> set = anchorItem(rect());

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

    setFlag(QGraphicsItem::ItemIsMovable, false);

    QGraphicsRectItem::mousePressEvent(event);
}

void GraphicsRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);

    QGraphicsRectItem::mouseReleaseEvent(event);
}

void GraphicsRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->pos();
    QRectF rectf = rect();

    switch (m_edge) {
    case Edge::TopLeft:
        rectf.setTopLeft(pos);
        setRect(rectf);
        break;
    case Edge::TopRight:
        rectf.setTopRight(pos);
        setRect(rectf);
        break;
    case Edge::BottomLeft:
        rectf.setBottomLeft(pos);
        setRect(rectf);
        break;
    case Edge::BottomRight:
        rectf.setBottomRight(pos);
        setRect(rectf);
        break;
    case Edge::Center:
        setFlag(QGraphicsItem::ItemIsMovable, true);
        break;
    default:
        break;
    }

    //adjustRect();

    QGraphicsRectItem::mouseMoveEvent(event);
}

void GraphicsRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos = event->pos();
    QVector<QRectF> set = anchorItem(rect());

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

    QGraphicsRectItem::hoverMoveEvent(event);
}
