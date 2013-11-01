  #include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);

    QThread* joystick_thread = new QThread;
    Joystick *joystick_worker = new Joystick(&vehicleConnection);
    joystick_worker->moveToThread(joystick_thread);
    joystick_thread->start();

    connect(ui->joystickDisconnectButton, SIGNAL( clicked() ),
                      joystick_worker, SLOT( detachJoystick() ), Qt::DirectConnection);
    connect(ui->joystickConnectButton, SIGNAL( clicked() ),
                      joystick_worker, SLOT( attachJoystick() ), Qt::AutoConnection);




    connect(&video1thread, SIGNAL(renderedImage(QGraphicsScene*)),
                 this, SLOT(updateVideo1Pixmap(QGraphicsScene*)));
    video1IsRunning = false;

    connect(&vehicleConnection, SIGNAL(connectionStatusChanged(int)),
            this, SLOT(updateConnectionStatus(int)));

    connect(joystick_worker, SIGNAL(joystickStateChanged(bool)),
            this, SLOT(updateJoystickStatus(bool)));



    ui->joystickComboBox->addItem("0");
    ui->joystickComboBox->addItem("1");

    ui->videoSourceSelection->addItem("Acrylic Port");
    ui->videoSourceSelection->addItem("Acrylic Starboard");

    ui->connectionStatusLabel->setText("Not Connected");
    ui->joystickStatusLabel->setText("Not Connected");
}

Widget::~Widget()
{
    delete ui;
}
/*

void Widget::on_joystickConnectButton_clicked()
{

    //moveToThread(joystick.currentThread());
    QMetaObject::invokeMethod(&joystick, "reattachJoystick", Qt::QueuedConnection);


    if(joystick.attached)
        QMetaObject::invokeMethod(&joystick, "reattachJoystick", Qt::QueuedConnectio);

}
*/
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
        video1thread.start(QThread::HighestPriority	);
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

void Widget::updateJoystickStatus(bool status) {
    if(status) {
        ui->joystickStatusLabel->setText("Connected");
        ui->joystickConnectButton->setDisabled(true);
        ui->joystickDisconnectButton->setDisabled(false);

        //ui->joystickConnectButton->setText("Disconnect");
    } else {
        ui->joystickStatusLabel->setText("Not Connected");
        ui->joystickConnectButton->setDisabled(false);
        ui->joystickDisconnectButton->setDisabled(true);
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
