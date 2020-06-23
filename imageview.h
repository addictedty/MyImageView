#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QtWidgets>
#include <QtCore>
#include <QtGui>
#include "graphicsrectitem.h"
#include "graphicslineitem.h"
#include "graphicscrossitem.h"
#include "graphicstextitem.h"

class ImageView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = nullptr);

    enum class CursorShape{
        NoDrag,             //无操作
        ScrollHandDrag,     //允许拖动
        RubberBandDrag,     //选择区域
    };

    enum class Command {
        None,
        Move,//移动
        ZoomIn,//放大
        ZoomOut,//缩小
        Rotation,//旋转
        Erasing,//擦除
        Drawing,//绘制
        Painting,//
        NB
    };

    enum class Shape{
        None,
        Rectangle,      //绘制矩形
        Line,           //绘制直线
        Cross,          //绘制十字
    };

public:
    /*! 原始大小/保持比例/适应屏幕 Qt::AspectRatioMode*/
    enum class ViewMode { Fit, Keep, Original };

    void setViewMode(ViewMode mode);            //设置视图显示模式
    void setCommand(Command cmd);               //设置操作指令
    void setCursorShape(CursorShape corShape);  //设置光标操作
    void setShape(Shape shape);                 //设置形状
    //设置鼠标缩放标志
    void setWheelZoom(bool flag);

    //设置pos点为窗口显示的中心的
    void setViewCenter(QPointF pos);

    //rectName:矩形索引名称
    //redraw:是否重新绘制标志   true:清除已存在矩形重新绘制    false:显示rectName矩形,重新编辑
    void drawRect(QString rectName, bool redraw = false);
    //获取绘制的矩形
    QRectF getDrawRect(QString rectName);
    //显示矩形
    void dispRect(QRectF rectf, QString rectName = "");
    //获取显示的矩形
    QRectF getDispRect(QString rectName);
    //隐藏矩形, 若rectName为空,则隐藏所有
    void hideRect(QString rectName = "");
    //删除矩形, 若rectName为空,则删除所有
    bool removeRect(QString rectName = "");

    //lineName:直线索引名称
    //redraw:是否重新绘制标志   true:清除已存在直线重新绘制    false:显示lineName直线,重新编辑
    void drawLine(QString lineName, bool redraw = false);
    //获取绘制的直线
    QLineF getDrawLine(QString lineName);
    //显示直线
    void dispLine(QLineF linef, QString lineName = "");
    //获取显示的直线
    QLineF getDispLine(QString lineName);
    //隐藏直线, 若lineName为空,则隐藏所有
    void hideLine(QString lineName = "");
    //删除直线, 若lineName为空,则删除所有
    bool removeLine(QString lineName = "");

    //crossName:十字索引名称
    //redraw:是否重新绘制标志   true:清除已存在十字重新绘制    false:显示lineName十字,重新编辑
    void drawCross(QString crossName, bool redraw = false);
    //获取绘制的十字
    bool getDrawCross(QPointF &center, qreal &width, qreal &height, QString crossName);
    //显示十字
    void dispCross(QPointF center, qreal width = 100, qreal height = 100, QString crossName = "");
    //获取显示的十字
    bool getDispCross(QPointF &center, qreal &width, qreal &height, QString crossName);
    //隐藏十字, 若crossName为空,则隐藏所有
    void hideCross(QString crossName = "");
    //删除十字, 若crossName为空,则删除所有
    bool removeCross(QString crossName = "");

    //显示文本
    void dispText(QString &text, QString textName = "");
    void dispText(QString &text, QPointF pos, int size = 9, QString textName = "");
    //隐藏文本, 若textName为空,则隐藏所有
    void hideText(QString textName = "");
    //删除文本, 若textName为空,则删除所有
    bool removeText(QString textName = "");

public slots:
    void fromImage(QImage image);       //加载图像

    void zoomRatio(double sx, double sy);  //
    void zoomIn();
    void zoomOut();

    void resetZoom();

signals:
    void drawRectFinished_signal(QImage image);     //矩形绘制完成时发送该信号,可连接该信号获得矩形区域图像
    void drawLineFinished_signal(QLineF linef);                 //直线绘制完成时发送该信号
    void drawCrossFinished_signal(QPointF center, qreal width, qreal height);//直线绘制完成时发送该信号

private slots:
    void drawRectFinish_slot();
    void drawLineFinish_slot();
    void drawCrossFinish_slot();

private:
    //记录鼠标按下时位置
    QPoint m_origin;
    // 橡皮筋
    QRubberBand *m_rubberBand = nullptr;

    //允许鼠标缩放
    bool m_wheelZoom = false;

    //显示模式
    ViewMode m_mode = ViewMode::Fit;
    Command m_cmd = Command::None;
    CursorShape m_corShape = CursorShape::NoDrag;
    Shape m_shape = Shape::None;

    QImage m_image;                 //原始图像
    QGraphicsPixmapItem *m_PixmapItem;

    QString m_rectName;             //矩形索引
    QMap<QString, GraphicsRectItem*> m_drawRectMap;     //绘制矩形列表
    GraphicsRectItem *m_drawRect;
    QMap<QString, GraphicsRectItem*> m_dispRectMap;     //显示矩形列表
    GraphicsRectItem *m_dispRect;

    QString m_lineName;             //直线索引
    QMap<QString, GraphicsLineItem*> m_drawLineMap;
    GraphicsLineItem *m_drawLine;
    QMap<QString, GraphicsLineItem*> m_dispLineMap;
    GraphicsLineItem *m_dispLine;

    QString m_crossName;             //十字索引
    QMap<QString, GraphicsCrossItem*> m_drawCrossMap;
    GraphicsCrossItem *m_drawCross;
    QMap<QString, GraphicsCrossItem*> m_dispCrossMap;
    GraphicsCrossItem *m_dispCross;

    QString m_textName;             //文本索引
    QMap<QString, GraphicsTextItem*> m_dispTextMap;
    GraphicsTextItem *m_dispTest;

private:
    QBrush &alphaTexture();
    QRect fft(QRubberBand *rubberBand);     //获取橡皮筋场景信息

protected:
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);

};


#endif // IMAGEVIEW_H
