#include "widget.h"
#include "ui_widget.h"

//#define MINLAT 40.7443957975362
//#define MAXLAT 40.7428351119306
//#define MINLON -74.0306854248047
//#define MAXLON -74.0282821655273

//#define MAXLON -81.7499542236328
//#define MINLON -81.7527008056641
//#define MAXLAT 24.5602388063527
//#define MINLAT 24.5621123556539

#define MINLON -81.745613
#define MAXLON -81.745021
#define MINLAT 24.584381
#define MAXLAT 24.583874

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);

    this->vehicleEnabled = false;

    orientationScene = new QGraphicsScene();
    ui->vehicleOrientationGV->setScene(orientationScene);
    vehicleOrientation = new OrientationGraphic();
    orientationScene->addItem(vehicleOrientation);

    QThread *diagnostics_connection_thread = new QThread;
    diagnosticsConnection = new VehicleConnection();

    diagnosticsConnection->moveToThread(diagnostics_connection_thread);
    diagnostics_connection_thread->start();

    QThread *mission_control_connection_thread = new QThread;
    missionControlConnection = new VehicleConnection();

    missionControlConnection->moveToThread(mission_control_connection_thread);
    mission_control_connection_thread->start();

    QThread *missionControlThread = new QThread;
    missionControlWorker = new MissionControl(missionControlConnection);
    missionControlWorker->moveToThread(missionControlThread);

    QThread *diagnosticsThread = new QThread;
    diagnosticsWorker = new Diagnostics(diagnosticsConnection);
    diagnosticsWorker->moveToThread(diagnosticsThread);

    connect(ui->modeSelection, SIGNAL(activated(int)), missionControlWorker, SLOT(setVehicleMode(int)));


    connect(diagnosticsThread, SIGNAL(started()), diagnosticsWorker, SLOT(process()));
    connect(diagnosticsWorker, SIGNAL(infoUpdated(float,float,float)), vehicleOrientation, SLOT(updateState(float,float,float)), Qt::AutoConnection);
    connect(diagnosticsWorker, SIGNAL(pidValuesUpdated(float,float,float)), this, SLOT(updatePidLabels(float,float,float)));
    connect(diagnosticsWorker, SIGNAL(newMapPointAvailable(QPointF)), this, SLOT(addPointToMapView(QPointF)));
    connect(diagnosticsWorker, SIGNAL(desiredDepthChanged(float)), this, SLOT(updateDesiredDepth(float)));
    diagnosticsThread->start();
    missionControlThread->start();




    QThread* joystick_thread = new QThread;
    Joystick *joystick_worker = new Joystick(missionControlConnection);

    joystick_worker->moveToThread(joystick_thread);
    joystick_thread->start();

    video1thread.set_port(9997);
    video1thread.set_ip("192.168.10.90");

    video2thread.set_port(9999);
    video2thread.set_ip("192.168.10.90");

    video3thread.set_port(9998);
    video3thread.set_ip("192.168.10.90");

    connect(ui->joystickDisconnectButton, SIGNAL( clicked() ),
                      joystick_worker, SLOT( detachJoystick() ), Qt::DirectConnection);
    connect(ui->joystickConnectButton, SIGNAL( clicked() ),
                      joystick_worker, SLOT( attachJoystick() ), Qt::AutoConnection);




    connect(&video1thread, SIGNAL(renderedImage(QGraphicsScene*)),
                 this, SLOT(updateVideo1Pixmap(QGraphicsScene*)));
    video1IsRunning = false;

    connect(&video2thread, SIGNAL(renderedImage(QGraphicsScene*)),
                 this, SLOT(updateVideo2Pixmap(QGraphicsScene*)));
    video2IsRunning = false;

    connect(&video3thread, SIGNAL(renderedImage(QGraphicsScene*)),
                 this, SLOT(updateVideo3Pixmap(QGraphicsScene*)));
    video3IsRunning = false;

    connect(missionControlConnection, SIGNAL(connectionStatusChanged(int)),
            this, SLOT(updateMissionControlConnectionStatus(int)));
    connect(diagnosticsConnection, SIGNAL(connectionStatusChanged(int)),
            this, SLOT(updateDiagnosticsConnectionStatus(int)));

    connect(joystick_worker, SIGNAL(joystickStateChanged(bool)),
            this, SLOT(updateJoystickStatus(bool)));


    connect(diagnosticsWorker, SIGNAL(thrustVectorChanged(float,float,float,float)),
            this, SLOT(updateVehicleThrustScene(float,float,float,float)));

    ui->joystickComboBox->addItem("0");
    ui->joystickComboBox->addItem("1");


    ui->diagnosticsStatusLabel->setText("Not Connected");
    ui->missionControlStatusLabel->setText("Not Connected");
    ui->joystickStatusLabel->setText("Not Connected");


    this->mapScene = new QGraphicsScene(0,0,1240,1158);
    this->mapImage = QImage(":images/fkcc_lagoon3.png");


    this->mapScene->addPixmap(QPixmap::fromImage(mapImage));
    ui->mapGraphicsView->setScene(this->mapScene);

    qDebug() << "Width = " << mapImage.width() << " | Height = " << mapImage.height();

    this->mapTracking = true;

    this->vehicleThrustScene = new QGraphicsScene(0,0,ui->vehicleThrustView->width(), ui->vehicleThrustView->height());
    ui->vehicleThrustView->setScene(this->vehicleThrustScene);

    //QHBoxLayout *feed1Layout = new QHBoxLayout();
    //video1GraphicsView = new QGraphicsView();
    //feed1Layout->addWidget(video1GraphicsView);
    //feed1Dialog.setLayout(feed1Layout);


}

Widget::~Widget()
{
    delete ui;
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

void Widget::updateVideo2Pixmap(QGraphicsScene *scene)
{
    ui->video2GV->setScene(scene);
    update();
}

void Widget::updateVideo3Pixmap(QGraphicsScene *scene)
{
    ui->video3GV->setScene(scene);
    update();
}

void Widget::on_startVideo1FeedButton_clicked()
{
    if(video1IsRunning) {
        video1thread.stop();
        //feed1Dialog.close();
        ui->startVideo1FeedButton->setText("Start Stream");
    } else {
        //feed1Dialog.show();
        video1thread.start(QThread::HighestPriority	);
        ui->startVideo1FeedButton->setText("Stop Stream");
    }
    video1IsRunning = !video1IsRunning;
}

void Widget::updateMissionControlConnectionStatus(int status) {
    if(status) {
        ui->missionControlStatusLabel->setText("Connected");
        ui->connectToMissionControlButton->setText("Disconnect from Vehicle");
    } else {
        ui->missionControlStatusLabel->setText("Not Connected");
        ui->connectToMissionControlButton->setText("Connect to Vehicle");
    }
}

void Widget::updateDiagnosticsConnectionStatus(int status) {
    if(status) {
        ui->diagnosticsStatusLabel->setText("Connected");
        ui->connectToDiagnosticsButton->setText("Stop Diagnostics");
    } else {
        ui->diagnosticsStatusLabel->setText("Not Connected");
        ui->connectToDiagnosticsButton->setText("Begin Diagnostics");
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


void Widget::on_startVideo2FeedButton_clicked()
{
    if(video2IsRunning) {
        video2thread.stop();
        ui->startVideo2FeedButton->setText("Start Stream");
    } else {
        video2thread.start(QThread::HighestPriority	);
        ui->startVideo2FeedButton->setText("Stop Stream");
    }
    video2IsRunning = !video2IsRunning;
}

void Widget::on_startVideo3FeedButton_clicked()
{
    if(video3IsRunning) {
        video3thread.stop();
        ui->startVideo3FeedButton->setText("Start Stream");
    } else {
        video3thread.start(QThread::HighestPriority	);
        ui->startVideo3FeedButton->setText("Stop Stream");
    }
    video3IsRunning = !video3IsRunning;
}

void Widget::on_connectToMissionControlButton_clicked()
{
    if(missionControlConnection->connected) {
        QMetaObject::invokeMethod(missionControlConnection, "disconnectFromVehicle", Qt::QueuedConnection);
    } else {
        QMetaObject::invokeMethod(missionControlConnection, "connectToVehicle", Qt::QueuedConnection,
                                  Q_ARG(QString, ui->ipAddressInput->text()),
                                  Q_ARG(QString, ui->missionControlInput->text()));
        missionControlWorker->setVehicleMode(0);
    }
}



void Widget::on_connectToDiagnosticsButton_clicked()
{
    if(diagnosticsConnection->connected) {
        QMetaObject::invokeMethod(diagnosticsConnection, "disconnectFromVehicle", Qt::QueuedConnection);
    } else {
        QMetaObject::invokeMethod(diagnosticsConnection, "connectToVehicle", Qt::QueuedConnection,
                                  Q_ARG(QString, ui->ipAddressInput->text()),
                                  Q_ARG(QString, ui->diagnosticsInput->text()));
    }
}


void Widget::on_pushButton_clicked()
{
    QMetaObject::invokeMethod(missionControlWorker, "setPIDValues", Qt::QueuedConnection,
                              Q_ARG(float, ui->kp_Textfield->text().toFloat()),
                              Q_ARG(float, ui->ki_Textfield->text().toFloat()),
                              Q_ARG(float, ui->kd_Textfield->text().toFloat()));
}

void Widget::updatePidLabels(float kp, float ki, float kd)
{
    QString kpStr;
    QString kiStr;
    QString kdStr;
    ui->kp_curVal->setText(kpStr.sprintf("%f", kp));
    ui->ki_curVal->setText(kiStr.sprintf("%f", ki));
    ui->kd_curVal->setText(kdStr.sprintf("%f", kd));

}

void Widget::on_pushButton_2_clicked()
{
    QMetaObject::invokeMethod(missionControlWorker, "updateCurrentOrientation", Qt::QueuedConnection);
}

void Widget::updateMapView() {
    int i;
    QPen pen(Qt::green);
    pen.setWidth(2);
    this->mapScene->clear();
    this->mapScene->addPixmap(QPixmap::fromImage(this->mapImage));
    for(i=1; i < std::min(pointVector.size(), 20); i++) {
        this->mapScene->addEllipse(pointVector[i-1].point.rx()-3, pointVector[i-1].point.ry()-3, 6, 6, pen);
        this->mapScene->addLine(pointVector[i-1].point.rx(), pointVector[i-1].point.ry(), pointVector[i].point.rx(), pointVector[i].point.ry(), pen);
    }
}

void Widget::addPointToMapView(QPointF point) {

    // 1792 x 1536
    float pixelY = ((point.rx() - MINLAT) / (MAXLAT - MINLAT)) * (1240 - 1);
    float pixelX = ((point.ry() - MINLON) / (MAXLON - MINLON)) * (1158 - 1);
    pixelX += 80;
    pixelY += 65;
    qDebug() << "Adding point : (" << pixelX << "," << pixelY << ")";
    if(this->mapTracking) {
        pointVector.prepend(GPSPoint(QPointF(pixelX, pixelY), QDateTime::currentDateTime()));
    }
    QString lonText;
    QString latText;
    ui->lattitudeBox->setText(latText.sprintf("%f", point.rx()));
    ui->longitudeBox->setText(lonText.sprintf("%f", point.ry()));
    updateMapView();
}

void Widget::removeAllPointsFromMapView() {
    pointVector.clear();
    //pointDateTime.clear();
    updateMapView();
}

void Widget::on_startStopTrackingButton_clicked()
{
    this->mapTracking = !this->mapTracking;

    if(this->mapTracking)
        ui->startStopTrackingButton->setText("Stop Tracking");
    else
        ui->startStopTrackingButton->setText("Start Tracking");
}

void Widget::on_resetMapButton_clicked()
{
    QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "Reset Map?", "Are you sure you'd like to remove all map points?",
                                   QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::Yes) {
       removeAllPointsFromMapView();
     } else {
        /* do nothing */
     }

}


void Widget::updateVehicleThrustScene(float s1, float s2, float s3, float h1) {
    QPen pen_red(Qt::red);
    QPen pen_black(Qt::black);
    QPen pen_green(Qt::green);
    pen_green.setWidth(3);

    //qDebug() << "Size = (" << this->vehicleThrustScene->width() << "," << this->vehicleThrustScene->height() << ")";

    this->vehicleThrustScene->clear();

    /* draw vehicle frame */
    this->vehicleThrustScene->addRect(40,40,20,130, pen_black);
    this->vehicleThrustScene->addRect(140,40,20,130, pen_black);
    this->vehicleThrustScene->addRect(65, 40, 70, 15, pen_black);


    /* draw thrust vectors */
    this->vehicleThrustScene->addLine(70,90,70,90-s1,pen_green);
    this->vehicleThrustScene->addLine(130,90,130,90-s2,pen_green);
    this->vehicleThrustScene->addLine(100,130,100,130-s3,pen_green);






}


void Widget::on_enableDisableVehicleButton_clicked()
{
    /* reverse the vehicle mode state */
    this->missionControlWorker->setVehicleMode(0);
    ui->modeSelection->setCurrentIndex(0);
}


void Widget::updateDesiredDepth(float depth) {
    QString depthStr;
    ui->currentDesiredDepth->setText(depthStr.sprintf("%f", depth));
}

void Widget::on_updateDesiredDepthButton_clicked()
{
    QMetaObject::invokeMethod(missionControlWorker, "updateDesiredDepth", Qt::QueuedConnection, Q_ARG(float, ui->desiredDepthInput->text().toFloat()));
}
