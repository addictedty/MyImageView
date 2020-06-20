#ifndef GRAPHICSCROSSITEM_H
#define GRAPHICSCROSSITEM_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include <QPen>

class GraphicsCrossItem : public QObject , public QGraphicsItem
{
    Q_OBJECT
    enum class Edge {
                None = 0x0,
                Top = 0x1,
                Bottom = 0x2,
                Left = 0x4,
                Right = 0x8,
                Center = 0x10,
                TopLeft = Edge::Top | Edge::Left,
                BottomLeft = Edge::Bottom | Edge::Left,
                TopRight = Edge::Top | Edge::Right,
                BottomRight = Edge::Bottom | Edge::Right
            };

public:
    GraphicsCrossItem(QGraphicsItem* parent = nullptr);
    GraphicsCrossItem(QPointF center, qreal width, qreal height);

signals:
    void drawFinished();

public:
    void setCross(const QPointF &center, qreal width, qreal height);
    bool m_isDrawing;

    QRectF getRect();

private:
    QPen m_pen;
    QRectF m_rect;
    QRectF m_boundingRect;

    Edge m_edge;

    QPointF m_rectCenter;

    //获取描点
    QVector<QRectF> anchorItem(const QRectF &rectf);

    void adjustRect();

protected:
    QRectF boundingRect() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // GRAPHICSCROSSITEM_H
