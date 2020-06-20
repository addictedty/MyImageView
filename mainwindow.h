#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnOpen_clicked();

    void on_btnZoonOut_clicked();

    void on_btnZoonIn_clicked();

    void on_btnResetSize_clicked();

    void on_checkBox_clicked(bool checked);

    void on_rbtnNoDrag_clicked();

    void on_btnDrawRect_clicked();

    void on_btnGetDrawRect_clicked();

    void on_btnDispRect_clicked();

    void on_btnGetDispRect_clicked();

    void on_btnDrawLine_clicked();

    void on_btnDispLine_clicked();

    void on_btnGetDrawLine_clicked();

    void on_btnGetDispLine_clicked();

    void on_btnDrawCross_clicked();

    void on_btnDispCross_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
