#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->rbtnScrollHandDrag, SIGNAL(clicked()), this, SLOT(on_rbtnNoDrag_clicked()));
    connect(ui->rbtnRubberBandDrag, SIGNAL(clicked()), this, SLOT(on_rbtnNoDrag_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpen_clicked()
{
    QImage image;
    QString fileName = QFileDialog::getOpenFileName(this, "选择图片", "", tr("Images (*.png *.bmp *.jpg)"));
    image.load(fileName);
    ui->graphicsView->fromImage(image);
}

void MainWindow::on_btnZoonOut_clicked()
{
    ui->graphicsView->zoomOut();
}

void MainWindow::on_btnZoonIn_clicked()
{
    ui->graphicsView->zoomIn();
}

void MainWindow::on_btnResetSize_clicked()
{
    ui->graphicsView->resetZoom();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    ui->graphicsView->setWheelZoom(checked);
}

void MainWindow::on_rbtnNoDrag_clicked()
{
    QRadioButton *rbtn = qobject_cast<QRadioButton*>(sender());
    if(rbtn->objectName() == ui->rbtnNoDrag->objectName())
        ui->graphicsView->setCursorShape(ImageView::CursorShape::NoDrag);
    else if(rbtn->objectName() == ui->rbtnScrollHandDrag->objectName())
        ui->graphicsView->setCursorShape(ImageView::CursorShape::ScrollHandDrag);
    else if(rbtn->objectName() == ui->rbtnRubberBandDrag->objectName())
        ui->graphicsView->setCursorShape(ImageView::CursorShape::RubberBandDrag);
}

void MainWindow::on_btnDrawRect_clicked()
{
    QString rectName = "rect1";
    ui->graphicsView->drawRect(rectName, true);
}

void MainWindow::on_btnGetDrawRect_clicked()
{
    QRectF rect = ui->graphicsView->getDrawRect("rect1");
    Q_UNUSED(rect);
}

void MainWindow::on_btnDispRect_clicked()
{
    ui->graphicsView->dispRect(QRectF(100, 100, 400, 300), "rect1");

    ui->graphicsView->dispRect(QRectF(600, 100, 400, 300), "rect2");

    ui->graphicsView->dispRect(QRectF(100, 500, 400, 300), "rect3");

    ui->graphicsView->dispRect(QRectF(600, 500, 400, 300), "rect4");
}

void MainWindow::on_btnGetDispRect_clicked()
{
    QRectF rect1 = ui->graphicsView->getDispRect("rect1");
    QRectF rect2 = ui->graphicsView->getDispRect("rect2");
    QRectF rect3 = ui->graphicsView->getDispRect("rect3");
    QRectF rect4 = ui->graphicsView->getDispRect("rect4");
    QRectF rect5 = ui->graphicsView->getDispRect("rect5");
    Q_UNUSED(rect1);
    Q_UNUSED(rect2);
    Q_UNUSED(rect3);
    Q_UNUSED(rect4);
    Q_UNUSED(rect5);
}

void MainWindow::on_btnDrawLine_clicked()
{
    QString lineName = "line1";
    ui->graphicsView->drawLine(lineName, false);
}

void MainWindow::on_btnDispLine_clicked()
{
    ui->graphicsView->dispLine(QLineF(10, 10, 3366, 10), "line1");
    ui->graphicsView->dispLine(QLineF(3366, 10, 3366, 2684), "line2");
    ui->graphicsView->dispLine(QLineF(3366, 2684, 10, 2684), "line3");
    ui->graphicsView->dispLine(QLineF(10, 2684, 10, 10), "line4");
}

void MainWindow::on_btnGetDrawLine_clicked()
{
    QLineF linef = ui->graphicsView->getDrawLine("line1");
    Q_UNUSED(linef);
}

void MainWindow::on_btnGetDispLine_clicked()
{
    QLineF linef1 = ui->graphicsView->getDispLine("line1");
    QLineF linef2 = ui->graphicsView->getDispLine("line2");
    QLineF linef3 = ui->graphicsView->getDispLine("line3");
    QLineF linef4 = ui->graphicsView->getDispLine("line4");
    QLineF linef5 = ui->graphicsView->getDispLine("line5");
    Q_UNUSED(linef1);
    Q_UNUSED(linef2);
    Q_UNUSED(linef3);
    Q_UNUSED(linef4);
    Q_UNUSED(linef5);
}

void MainWindow::on_btnDrawCross_clicked()
{
    ui->graphicsView->drawCross("cross1", false);
}

void MainWindow::on_btnDispCross_clicked()
{
    ui->graphicsView->dispCross(QPointF(1688, 1352), 500, 200);
}

void MainWindow::on_btnGetDrawCross_clicked()
{

}

void MainWindow::on_btnGetDispCross_clicked()
{

}

void MainWindow::on_btnDispText1_clicked()
{
    QString text = "显示文字\r\n12345";
    ui->graphicsView->dispText(text, QPointF(100, 100), 100, "text1");
}

void MainWindow::on_btnDispText2_clicked()
{
    QString text = "显示文字\r\n12345";
    ui->graphicsView->dispText(text, QPointF(200, 200), 50, "text1");
}
