#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QColor>
#include "picture.h"
#include "cloning.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage* loadImage();

public slots:
    void loadSource();
    void loadBackground();
    void generateResult();
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void clearSelection();
    void previewResult();
private:
    Ui::MainWindow *ui;
    QImage* background, *source;
    QImage sourceplus;
    Picture * pic;
};

#endif // MAINWINDOW_H
