#ifndef GRAPHICSLINEITEM_H
#define GRAPHICSLINEITEM_H

#include <QObject>
#include <QWidget>
#include <QGraphicsLineItem>
#include <QPen>
#include <QFont>

class GraphicsLineItem : public QObject , public QGraphicsLineItem
{
    Q_OBJECT

    enum class Edge {
        None = 0x0,
        Top = 0x1,
        Bottom = 0x2,
        Center = 0x4
    };

public:
    GraphicsLineItem(QGraphicsLineItem* parent = nullptr);
    GraphicsLineItem(QLineF linef);

signals:
    void drawFinished();

public:
    bool m_isDrawing;

private:
    QPen m_pen;

    QLineF m_line;

    Edge m_edge;

    //获取描点
    QVector<QRectF> anchorItem(const QLineF &linef);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // GRAPHICSLINEITEM_H
