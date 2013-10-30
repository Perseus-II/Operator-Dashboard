#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    connect(&video1thread, SIGNAL(renderedImage(QGraphicsScene*)),
                 this, SLOT(updateVideo1Pixmap(QGraphicsScene*)));
    video1IsRunning = false;

    ui->setupUi(this);
    ui->joystickComboBox->addItem("/dev/video0");
    ui->joystickComboBox->addItem("/dev/video1");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_connectButton_clicked()
{
    qDebug() << "Connect button clicked";
}

void Widget::on_rescanButton_clicked()
{
    qDebug() << "Rescan button clicked";
}

void Widget::updateVideo1Pixmap(QGraphicsScene *scene)
{

    ui->video1GV->setScene(scene);
    update();
}

void Widget::on_startVideo1FeedButton_clicked()
{
    if(video1IsRunning) {
        video1thread.stop();
        ui->startVideo1FeedButton->setText("Start Stream");
    } else {
        video1thread.start();
        ui->startVideo1FeedButton->setText("Stop Stream");
    }
    video1IsRunning = !video1IsRunning;
}

