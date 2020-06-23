#ifndef GRAPHICSTEXTITEM_H
#define GRAPHICSTEXTITEM_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QPen>

class GraphicsTextItem : public QGraphicsItem
{
public:
    GraphicsTextItem(QGraphicsItem *parent = nullptr);

    GraphicsTextItem(QString &text, QPointF pos, int pointSize, QGraphicsItem *parent = nullptr);

public:
    void setTextColor(QColor color);
    void setText(QString text, QPointF pos, int pointSize);
    void setPointSize(int pointSize);
    void setFont(QFont &font);

    void setText(const QString &text);

private:
    QRectF m_rect;

    QGraphicsTextItem *m_text;

    QFont font;

    QPen pen;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // GRAPHICSTEXTITEM_H
