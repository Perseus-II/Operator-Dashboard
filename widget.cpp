#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    joystick.start(); /* start joystick thread */
    connect(&video1thread, SIGNAL(renderedImage(QGraphicsScene*)),
                 this, SLOT(updateVideo1Pixmap(QGraphicsScene*)));
    video1IsRunning = false;

    connect(&vehicleConnection, SIGNAL(connectionStatusChanged(int)),
            this, SLOT(updateConnectionStatus(int)));

    ui->setupUi(this);
    ui->joystickComboBox->addItem("/dev/video0");
    ui->joystickComboBox->addItem("/dev/video1");

    ui->videoSourceSelection->addItem("Acrylic Port");
    ui->videoSourceSelection->addItem("Acrylic Starboard");

    ui->connectionStatusLabel->setText("Not connected");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_connectButton_clicked()
{
    qDebug() << "Connect button clicked";
    joystick.attachJoystick(0);

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

void Widget::updateConnectionStatus(int status) {
    if(status) {
        ui->connectionStatusLabel->setText("Connected");
        ui->connectToVehicleButton->setText("Disconnect from Vehicle");
    } else {
        ui->connectionStatusLabel->setText("Not Connected");
        ui->connectToVehicleButton->setText("Connect to Vehicle");
    }
}


void Widget::on_connectToVehicleButton_clicked()
{
    if(vehicleConnection.connected) {
        vehicleConnection.disconnectFromVehicle();
    } else {
        vehicleConnection.connectToVehicle(
                    ui->ipAddressInput->text(),
                    ui->missionControlInput->text(),
                    ui->diagnosticsInput->text());
    }
}
