#include "imageview.h"
#include <QDebug>

static const char * const xpm_alpha_texture[] = {
    "4 4 2 1",
    "0 c #fff",
    "1 c #ccc",
    "0101",
    "1010",
    "0101",
    "1010"
};

ImageView::ImageView(QWidget *parent) : QGraphicsView(new QGraphicsScene, parent)
{
    m_PixmapItem = nullptr;

    setBackgroundBrush(alphaTexture());//设置背景纹理
    setCacheMode(QGraphicsView::CacheBackground);//设置背景缓存机制

    //不禁用会出现图像无法放大的问题
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//禁用垂直滚动栏
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//禁用水平滚动栏

    setDragMode(QGraphicsView::NoDrag);//设置禁止拖动/NoDrag/ScrollHandDrag/RubberBandDrag
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);   //消除绘图出现的重影
}

void ImageView::drawRectFinish_slot()
{
    QRectF rectf = m_drawRect->mapRectToScene(m_drawRect->rect());
    QRect irect(rectf.x(), rectf.y(), rectf.width(), rectf.height());

    emit drawRectFinished_signal(m_image.copy(irect));
}

void ImageView::drawLineFinish_slot()
{
    emit drawLineFinished_signal(m_drawLine->line());
}

void ImageView::drawCrossFinish_slot()
{
    QRectF rectf = m_drawCross->mapRectToScene(m_drawCross->getRect());
    emit drawCrossFinished_signal(rectf.center(), rectf.width(), rectf.height());
}

void ImageView::setViewMode(ImageView::ViewMode mode)
{
    m_mode = mode;
    if(m_PixmapItem)
        fitInView(m_PixmapItem, static_cast<Qt::AspectRatioMode>(m_mode));
}

void ImageView::setCommand(ImageView::Command cmd)
{
    m_cmd = cmd;
}

void ImageView::setCursorShape(ImageView::CursorShape corShape)
{
    m_corShape = corShape;
    setDragMode(static_cast<QGraphicsView::DragMode>(corShape));
}

void ImageView::setShape(ImageView::Shape shape)
{
    m_shape = shape;
}

void ImageView::setWheelZoom(bool flag)
{
    m_wheelZoom = flag;
}

void ImageView::setViewCenter(QPointF pos)
{
    centerOn(pos);
}

void ImageView::drawRect(QString rectName, bool redraw)
{
    if(m_drawRectMap.contains(m_rectName))
    {
        if(redraw)
        {
            m_drawRect = m_drawRectMap.value(rectName);
            scene()->removeItem(m_drawRect);
            m_drawRectMap.remove(rectName);
        }
        else
        {
            m_drawRect = m_drawRectMap.value(rectName);
            m_drawRect->setEnabled(true);
            m_drawRect->setVisible(true);
            return;
        }
    }
    setCommand(ImageView::Command::Drawing);
    setShape(ImageView::Shape::Rectangle);
    m_rectName = rectName;

    setDragMode(QGraphicsView::NoDrag);
}

QRectF ImageView::getDrawRect(QString rectName)
{
    if(m_drawRectMap.contains(rectName))
    {
        m_drawRect = m_drawRectMap.value(rectName);
        return m_drawRect->mapRectToScene(m_drawRect->rect());
    }
    else
        return QRectF();
}

void ImageView::drawLine(QString lineName, bool redraw)
{
    if(m_drawLineMap.contains(m_lineName))
    {
        if(redraw)
        {
            m_drawLine = m_drawLineMap.value(lineName);
            scene()->removeItem(m_drawLine);
            m_drawLineMap.remove(lineName);
        }
        else
        {
            m_drawLine = m_drawLineMap.value(lineName);
            m_drawLine->setEnabled(true);
            m_drawLine->setVisible(true);
            return;
        }
    }
    setCommand(ImageView::Command::Drawing);
    setShape(ImageView::Shape::Line);
    m_lineName = lineName;

    setDragMode(QGraphicsView::NoDrag);
}

QLineF ImageView::getDrawLine(QString lineName)
{
    if(m_drawLineMap.contains(lineName))
    {
        m_drawLine = m_drawLineMap.value(lineName);
        QPointF p1 = m_drawLine->mapToScene(m_drawLine->line().p1());
        QPointF p2 = m_drawLine->mapToScene(m_drawLine->line().p2());
        return QLineF(p1, p2);
    }
    else
        return QLineF();
}

void ImageView::dispRect(QRectF rectf, QString rectName)
{
    if(rectName.isEmpty())
    {
        m_dispRect = new GraphicsRectItem(rectf);
        scene()->addItem(m_dispRect);
    }
    else
    {
        if(m_dispRectMap.contains(rectName))
        {
            m_dispRect = m_dispRectMap.value(rectName);
            m_dispRect->setRect(rectf);
        }
        else
        {
            m_dispRect = new GraphicsRectItem(rectf);
            m_dispRectMap.insert(rectName, m_dispRect);
            scene()->addItem(m_dispRect);
        }
    }
}

QRectF ImageView::getDispRect(QString rectName)
{
    if(m_dispRectMap.contains(rectName))
    {
        m_drawRect = m_dispRectMap.value(rectName);
        return m_drawRect->mapRectToScene(m_drawRect->rect());
    }
    else
        return QRectF();
}

void ImageView::hideRect(QString rectName)
{
    if(rectName.isEmpty())
    {
        QList<GraphicsRectItem*> listItem = m_dispRectMap.values();

        foreach (GraphicsRectItem *var, listItem)
            var->setVisible(false);
    }
    else if(m_dispRectMap.contains(rectName))
    {
        m_dispRect = m_dispRectMap.value(rectName);
        m_dispRect->setVisible(false);
    }
}

bool ImageView::removeRect(QString rectName)
{
    if(rectName.isEmpty())
    {
        QList<GraphicsRectItem*> listItem = m_dispRectMap.values();

        foreach (GraphicsRectItem *var, listItem)
            scene()->removeItem(var);
        return true;
    }
    else if(m_dispRectMap.contains(rectName))
    {
        m_dispRect = m_dispRectMap.value(rectName);
        scene()->removeItem(m_dispRect);
        m_dispRectMap.remove(rectName);
        return true;
    }

    return false;
}

void ImageView::dispLine(QLineF linef, QString lineName)
{
    if(lineName.isEmpty())
    {
        m_dispLine = new GraphicsLineItem(linef);
        scene()->addItem(m_dispLine);
    }
    else
    {
        if(m_dispLineMap.contains(lineName))
        {
            m_dispLine = m_dispLineMap.value(lineName);
            m_dispLine->setLine(linef);
        }
        else
        {
            m_dispLine = new GraphicsLineItem(linef);
            m_dispLineMap.insert(lineName, m_dispLine);
            scene()->addItem(m_dispLine);
        }
    }
}

QLineF ImageView::getDispLine(QString lineName)
{
    if(m_dispLineMap.contains(lineName))
    {
        m_dispLine = m_dispLineMap.value(lineName);
        QPointF p1 = m_dispLine->mapToScene(m_dispLine->line().p1());
        QPointF p2 = m_dispLine->mapToScene(m_dispLine->line().p2());
        return QLineF(p1, p2);
    }
    else
        return QLineF();
}

void ImageView::hideLine(QString lineName)
{
    if(lineName.isEmpty())
    {
        QList<GraphicsLineItem*> listItem = m_dispLineMap.values();

        foreach (GraphicsLineItem *var, listItem)
            var->setVisible(false);
    }
    else if(m_dispLineMap.contains(lineName))
    {
        m_dispLine = m_dispLineMap.value(lineName);
        m_dispLine->setVisible(false);
    }
}

bool ImageView::removeLine(QString lineName)
{
    if(lineName.isEmpty())
    {
        QList<GraphicsLineItem*> listItem = m_dispLineMap.values();

        foreach (GraphicsLineItem *var, listItem)
            scene()->removeItem(var);
        return true;
    }
    else if(m_dispLineMap.contains(lineName))
    {
        m_dispLine = m_dispLineMap.value(lineName);
        scene()->removeItem(m_dispLine);
        m_dispCrossMap.remove(lineName);
        return true;
    }

    return false;
}

void ImageView::drawCross(QString crossName, bool redraw)
{
    if(m_drawCrossMap.contains(crossName))
    {
        if(redraw)
        {
            m_drawCross = m_drawCrossMap.value(crossName);
            scene()->removeItem(m_drawCross);
            m_drawCrossMap.remove(crossName);
        }
        else
        {
            m_drawCross = m_drawCrossMap.value(crossName);
            m_drawCross->setEnabled(true);
            m_drawCross->setVisible(true);
            return;
        }
    }
    setCommand(ImageView::Command::Drawing);
    setShape(ImageView::Shape::Cross);
    m_crossName = crossName;

    setDragMode(QGraphicsView::NoDrag);
}

bool ImageView::getDrawCross(QPointF &center, qreal &width, qreal &height, QString crossName)
{
    if(m_drawCrossMap.contains(crossName))
    {
        m_drawCross = m_drawCrossMap.value(crossName);
        center = m_drawCross->getRect().center();
        width = m_drawCross->getRect().width();
        height = m_drawCross->getRect().height();
        return true;
    }

    return false;
}

void ImageView::dispCross(QPointF center, qreal width, qreal height, QString crossName)
{
    if(crossName.isEmpty())
    {
        m_dispCross = new GraphicsCrossItem(center, width, height);
        scene()->addItem(m_dispCross);
    }
    else
    {
        if(m_dispCrossMap.contains(crossName))
        {
            m_dispCross = m_dispCrossMap.value(crossName);
            m_dispCross->setCross(center, width, height);
        }
        else
        {
            m_dispCross = new GraphicsCrossItem(center, width, height);
            m_dispCrossMap.insert(crossName, m_dispCross);
            scene()->addItem(m_dispCross);
        }
    }
}

bool ImageView::getDispCross(QPointF &center, qreal &width, qreal &height, QString crossName)
{
    if(m_dispCrossMap.contains(crossName))
    {
        m_dispCross = m_dispCrossMap.value(crossName);
        center = m_dispCross->getRect().center();
        width = m_dispCross->getRect().width();
        height = m_dispCross->getRect().height();
        return true;
    }

    return false;
}

void ImageView::hideCross(QString crossName)
{
    if(crossName.isEmpty())
    {
        QList<GraphicsCrossItem*> listItem = m_dispCrossMap.values();

        foreach (GraphicsCrossItem *var, listItem)
            var->setVisible(false);
    }
    else if(m_dispCrossMap.contains(crossName))
    {
        m_dispCross = m_dispCrossMap.value(crossName);
        m_dispCross->setVisible(false);
    }
}

bool ImageView::removeCross(QString crossName)
{
    if(crossName.isEmpty())
    {
        QList<GraphicsCrossItem*> listItem = m_dispCrossMap.values();

        foreach (GraphicsCrossItem *var, listItem)
            scene()->removeItem(var);
        return true;
    }
    else if(m_dispCrossMap.contains(crossName))
    {
        m_dispCross = m_dispCrossMap.value(crossName);
        scene()->removeItem(m_dispCross);
        m_dispCrossMap.remove(crossName);
        return true;
    }

    return false;
}

void ImageView::dispText(QString &text, QString textName)
{
    if(textName.isEmpty())
    {
        if(m_dispTest == nullptr)
            return;
        m_dispTest->setText(text);
    }
    else
    {
        if(m_dispTextMap.contains(textName))
        {
            m_dispTest = m_dispTextMap.value(textName);
            m_dispTest->setText(text);
        }
    }
}

void ImageView::dispText(QString &text, QPointF pos, int size, QString textName)
{
    if(textName.isEmpty())
    {
        m_dispTest = new GraphicsTextItem(text, pos, size);
        m_dispTest->setPos(pos);
        scene()->addItem(m_dispTest);
    }
    else
    {
        if(m_dispTextMap.contains(textName))
        {
            m_dispTest = m_dispTextMap.value(textName);
            m_dispTest->setText(text, pos, size);
        }
        else
        {
            m_dispTest = new GraphicsTextItem(text, pos, size);
            m_dispTextMap.insert(textName, m_dispTest);
            scene()->addItem(m_dispTest);
        }
    }
}

void ImageView::hideText(QString textName)
{
    if(textName.isEmpty())
    {
        QList<GraphicsTextItem*> listItem = m_dispTextMap.values();

        foreach (GraphicsTextItem *var, listItem)
            var->setVisible(false);
    }
    else if(m_dispTextMap.contains(textName))
    {
        m_dispTest = m_dispTextMap.value(textName);
        m_dispTest->setVisible(false);
    }
}

bool ImageView::removeText(QString textName)
{
    if(textName.isEmpty())
    {
        QList<GraphicsTextItem*> listItem = m_dispTextMap.values();

        foreach (GraphicsTextItem *var, listItem)
            scene()->removeItem(var);
        return true;
    }
    else if(m_dispTextMap.contains(textName))
    {
        m_dispTest = m_dispTextMap.value(textName);
        scene()->removeItem(m_dispTest);
        m_dispTextMap.remove(textName);
        return true;
    }

    return false;
}

void ImageView::fromImage(QImage image)
{
    if(m_PixmapItem)
    {
        m_PixmapItem->setPixmap(QPixmap::fromImage(image));

        //图像大小改变时，改变场景大小
        if(m_image.size() != image.size())
        {
            fitInView(m_PixmapItem, static_cast<Qt::AspectRatioMode>(m_mode));
            scene()->setSceneRect(0, 0, image.width(), image.height());
        }

    }
    else
    {
        m_PixmapItem = scene()->addPixmap(QPixmap::fromImage(image));
        m_PixmapItem->setTransformationMode(Qt::FastTransformation);
        m_PixmapItem->setZValue(-1);        //设置图像处于最底层
        fitInView(m_PixmapItem, static_cast<Qt::AspectRatioMode>(m_mode));
        scene()->setSceneRect(0, 0, image.width(), image.height());
    }

    m_image = image;
}

void ImageView::zoomRatio(double sx, double sy)
{
    if(!m_PixmapItem)
        return;
    fitInView(m_PixmapItem, static_cast<Qt::AspectRatioMode>(m_mode));

    scale(sx, sy);
}

void ImageView::zoomIn()
{
    scale(1.2, 1.2);
    update();
}

void ImageView::zoomOut()
{
    scale(1 / 1.2, 1 / 1.2);
    update();
}

void ImageView::resetZoom()
{
    fitInView(m_PixmapItem, static_cast<Qt::AspectRatioMode>(m_mode));
}

//void ImageView::connectInit()
//{

//}

QBrush &ImageView::alphaTexture()
{
    static QImage image(xpm_alpha_texture);
    static QBrush brush;
    brush.setTextureImage(image);
    brush.setTransform(QTransform::fromScale(30, 30));
    return brush;
}

QRect ImageView::fft(QRubberBand *rubberBand)
{
    int x = rubberBand->x();
    int y = rubberBand->y();
    int w = rubberBand->width();
    int h = rubberBand->height();

    //mapFromScene:将图像坐标转换为窗口坐标
    auto origin = mapFromScene(0, 0);
    QRect rect = QRect(x - origin.x(), y - origin.y(), w, h);
    return transform().inverted().mapRect(rect);
}

void ImageView::resizeEvent(QResizeEvent *event)
{
    if(m_PixmapItem)
        fitInView(m_PixmapItem, static_cast<Qt::AspectRatioMode>(m_mode));
    QGraphicsView::resizeEvent(event);
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    QPoint point = event->angleDelta();
    if(!m_PixmapItem)
        return;
    if(m_wheelZoom)
    {
        if(point.y() > 0)
            zoomIn();
        else
            zoomOut();
    }
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    if(m_cmd == Command::Drawing && m_shape == Shape::Rectangle)
    {
        m_origin = event->pos();
        m_rubberBand->setGeometry(QRect(m_origin, QSize()));
        m_rubberBand->show();
    }
    if(m_cmd == Command::Drawing && m_shape == Shape::Line)
    {
        m_origin = event->pos();
        m_rubberBand->setGeometry(QRect(m_origin, QSize()));
        m_rubberBand->show();
    }
    if(m_cmd == Command::Drawing && m_shape == Shape::Cross)
    {
        m_origin = event->pos();
        m_rubberBand->setGeometry(QRect(m_origin, QSize()));
        m_rubberBand->show();
    }

    QGraphicsView::mousePressEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_cmd == Command::Drawing && m_shape == Shape::Rectangle)
    {
        m_rubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());
    }
    if(m_cmd == Command::Drawing && m_shape == Shape::Line)
    {
        m_rubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());
    }
    if(m_cmd == Command::Drawing && m_shape == Shape::Cross)
    {
        m_rubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());
    }

    QGraphicsView::mouseMoveEvent(event);
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_cmd == Command::Drawing && m_shape == Shape::Rectangle)
    {
        m_rubberBand->hide();

        QRect rect_fft = fft(m_rubberBand);

        if(m_drawRectMap.contains(m_rectName))
            m_drawRect = m_drawRectMap.value(m_rectName);
        else
        {
            m_drawRect = new GraphicsRectItem;
            connect(m_drawRect, SIGNAL(drawFinished()), this, SLOT(drawRectFinish_slot()));
        }
        m_drawRect->setRect(rect_fft);
        scene()->addItem(m_drawRect);

        m_drawRectMap.insert(m_rectName, m_drawRect);

        m_cmd = Command::None;
        m_shape = Shape::None;

        setCursorShape(m_corShape);
    }

    if(m_cmd == Command::Drawing && m_shape == Shape::Line)
    {
        m_rubberBand->hide();

        QRect rect_fft = fft(m_rubberBand);

        if(m_drawLineMap.contains(m_lineName))
            m_drawLine = m_drawLineMap.value(m_lineName);
        else
        {
            m_drawLine = new GraphicsLineItem;
            connect(m_drawLine, SIGNAL(drawFinished()), this, SLOT(drawLineFinish_slot()));
        }
        m_drawLine->setLine(QLineF(rect_fft.topLeft(), rect_fft.bottomRight()));
        scene()->addItem(m_drawLine);

        m_drawLineMap.insert(m_lineName, m_drawLine);

        m_cmd = Command::None;
        m_shape = Shape::None;

        setCursorShape(m_corShape);
    }

    if(m_cmd == Command::Drawing && m_shape == Shape::Cross)
    {
        m_rubberBand->hide();

        QRect rect_fft = fft(m_rubberBand);

        if(m_drawCrossMap.contains(m_crossName))
            m_drawCross = m_drawCrossMap.value(m_crossName);
        else
        {
            m_drawCross = new GraphicsCrossItem;
            connect(m_drawCross, SIGNAL(drawFinished()), this, SLOT(drawCrossFinish_slot()));
        }
        m_drawCross->setCross(rect_fft.center(), rect_fft.width(), rect_fft.height());
        scene()->addItem(m_drawCross);

        m_drawCrossMap.insert(m_crossName, m_drawCross);

        m_cmd = Command::None;
        m_shape = Shape::None;

        setCursorShape(m_corShape);
    }

    QGraphicsView::mouseReleaseEvent(event);
}
