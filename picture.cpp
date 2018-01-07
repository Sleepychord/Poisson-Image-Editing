#include "picture.h"
#include "ui_picture.h"

Picture::Picture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Picture)
{
    ui->setupUi(this);
}

Picture::~Picture()
{
    delete ui;
}

void Picture::setImage(const QImage &img)
{
    this->image = img;
}

void Picture::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if(!this->image.isNull())
        painter.drawImage(10, 10, this->image);
}
