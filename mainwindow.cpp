#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->backgroundButton,SIGNAL(clicked()), this, SLOT(loadBackground()));
    connect(ui->sourceButton, SIGNAL(clicked(bool)), this, SLOT(loadSource()));
    connect(ui->clearSelectButton, SIGNAL(clicked(bool)),this, SLOT(clearSelection()));
    connect(ui->previewButton, SIGNAL(clicked(bool)), this, SLOT(previewResult()));
    connect(ui->generateButton, SIGNAL(clicked(bool)), this, SLOT(generateResult()));    
    this->background = this->source = NULL;
    this->setMouseTracking(true);
    this->pic = new Picture();
    this->pic->hide();
    //TODO
}
MainWindow::~MainWindow()
{
    delete ui;
}

QImage* MainWindow::loadImage(){
    auto filedir = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
            tr("Images (*.png *.xpm *.jpg *.tiff *.bmp)"));
    qDebug() << filedir << endl;
    auto img = new QImage(filedir);
    *img = img->scaledToWidth(300);
    return img;
}

void MainWindow::loadBackground(){
    this->background = loadImage();
    update();
}

void MainWindow::loadSource(){
    this->source = loadImage();
    this->sourceplus = this->source->copy();
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    if(this->background)
        painter.drawImage(500, 20, *(this->background));
    if(this->source)
        painter.drawImage(180, 20, this->sourceplus);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && this->source){
        if(event->x() >= 180 && event->x() <= 180 + this->source->width()
                && event->y() >= 20 && event->y() <= 20 + this->source->height()){
            const int maxdis = 2;
            int x = event->x() - 180, y = event->y() - 20;
            for(int i = x - maxdis;i < x + maxdis;i++)
                for(int j = y - maxdis;j < y + maxdis;j++)
                if(i >= 0 && i < this->sourceplus.width() && j >= 0 && j < this->sourceplus.height())
                    this->sourceplus.setPixel(i, j, qRgb(255,0,0));
        }
    }
    update();
}

void MainWindow::clearSelection()
{
    this->sourceplus = this->source->copy();
    update();
}

void MainWindow::previewResult()
{
    this->pic->setImage(Cloning::directCloning(sourceplus,*background));
    this->pic->show();
}
void MainWindow::generateResult()
{
    this->pic->setImage(Cloning::seamlessCloning(sourceplus,*background, *source));
    this->pic->show();    
}
