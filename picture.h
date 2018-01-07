#ifndef PICTURE_H
#define PICTURE_H

#include <QDialog>
#include <QImage>
#include <QPainter>
namespace Ui {
class Picture;
}

class Picture : public QDialog
{
    Q_OBJECT
    
public:
    explicit Picture(QWidget *parent = 0);
    ~Picture();
public slots:
    void setImage(const QImage& img);
    void paintEvent(QPaintEvent *event);
private:
    Ui::Picture *ui;
    QImage image;
};

#endif // PICTURE_H
