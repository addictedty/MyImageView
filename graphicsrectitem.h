#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include <QObject>
#include <QWidget>
#include <QGraphicsRectItem>
#include <QPen>

class GraphicsRectItem : public QObject , public QGraphicsRectItem
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
    GraphicsRectItem(QGraphicsRectItem* parent = nullptr);
    GraphicsRectItem(QRectF rectf);

signals:
    void drawFinished();

public:
    void showCross(bool flag = true);
    bool m_isDrawing;

private:
    QPen m_pen;

    //记录鼠标按下时位置
    QPointF m_origin;

    QRectF m_rect;

    Edge m_edge;

    bool m_showCross = true;

    //获取描点
    QVector<QRectF> anchorItem(const QRectF &rectf);

    void adjustRect();

protected:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

};

#endif // GRAPHICSRECTITEM_H
