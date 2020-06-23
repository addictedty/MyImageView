#include "graphicstextitem.h"
#include <QTextDocument>
#include <QPainter>
#include <QDebug>

GraphicsTextItem::GraphicsTextItem(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    pen.setColor(Qt::red);
    pen.setWidth(5);
}

GraphicsTextItem::GraphicsTextItem(QString &text, QPointF pos, int pointSize, QGraphicsItem *parent)
{
    pen.setColor(Qt::red);
    pen.setWidth(5);

    m_text = new QGraphicsTextItem(text, parent);
    m_text->setDefaultTextColor(QColor(Qt::red));
    font = m_text->font();
    font.setPointSize(pointSize);
    m_text->setFont(font);
    m_text->setParentItem(this);

    m_text->setPos(mapFromScene(pos));
    m_text->adjustSize();
    QRectF temp = m_text->mapRectToParent(m_text->boundingRect());
    m_rect.setRect(temp.left(), temp.top(),
                   temp.width(), temp.height());

    prepareGeometryChange();
}

void GraphicsTextItem::setText(QString text, QPointF pos, int pointSize)
{
    font = m_text->font();
    font.setPointSize(pointSize);
    m_text->setFont(font);

    m_text->setPlainText(text);
    m_text->adjustSize();
    m_text->setPos(mapFromScene(pos));

    QRectF temp = m_text->mapRectToParent(m_text->boundingRect());
    m_rect.setRect(temp.left(), temp.top(),
                   temp.width(), temp.height());

    prepareGeometryChange();
}

void GraphicsTextItem::setFont(QFont &font)
{

}

void GraphicsTextItem::setPointSize(int pointSize)
{

}

void GraphicsTextItem::setText(const QString &text)
{
    m_text->setPlainText(text);
}

QRectF GraphicsTextItem::boundingRect() const
{
    return m_text->mapRectToParent(m_text->boundingRect());
}

void GraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen);

    painter->drawLine(m_rect.topLeft(), m_rect.topRight());
    painter->drawLine(m_rect.bottomLeft(), m_rect.bottomRight());
}
