#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "videofeed.h"
#include <QPixmap>
#include <QDebug>
#include <QGraphicsScene>
#include "joystick.h"
#include "vehicleconnection.h"
#include "orientationgraphic.h"
#include <QMetaObject>
#include "diagnostics.h"
#include "missioncontrol.h"
#include <stdlib.h>
#include <QDir>
#include <QPointF>
#include <QPen>
#include <QVector>
#include <QDateTime>
#include <QMessageBox>
#include "gpspoint.h"
#include <QDialog>

#define MODE_DISABLED 0
#define MODE_MANUAL 1
#define MODE_AUTON 2
#define MODE_AUTON_DEPTH 3

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    //void on_joystickConnectButton_clicked();
    void on_rescanButton_clicked();
    void updateVideo1Pixmap(QGraphicsScene *scene);
    void updateVideo2Pixmap(QGraphicsScene *scene);
    void updateVideo3Pixmap(QGraphicsScene *scene);
    void on_startVideo1FeedButton_clicked();
    void on_startVideo2FeedButton_clicked();
    void updateMissionControlConnectionStatus(int status);
    void updateDiagnosticsConnectionStatus(int status);
    void updateJoystickStatus(bool status);
    void on_startVideo3FeedButton_clicked();
    void on_connectToMissionControlButton_clicked();
    void on_connectToDiagnosticsButton_clicked();
    void on_pushButton_clicked();
    void updatePidLabels(float,float,float);
    void on_pushButton_2_clicked();
    void addPointToMapView(QPointF);
    void removeAllPointsFromMapView();
    void updateMapView();
    void on_startStopTrackingButton_clicked();
    void on_resetMapButton_clicked();
    void updateVehicleThrustScene(float,float,float,float);
    void updateDesiredDepth(float);


    void on_enableDisableVehicleButton_clicked();

    void on_updateDesiredDepthButton_clicked();

private:
    Ui::Widget *ui;
    VehicleConnection *missionControlConnection;
    VehicleConnection *diagnosticsConnection;

    Diagnostics *diagnosticsWorker;
    MissionControl *missionControlWorker;

    VideoFeed video1thread;
    int video1IsRunning;
    VideoFeed video2thread;
    int video2IsRunning;
    VideoFeed video3thread;
    int video3IsRunning;

    OrientationGraphic *vehicleOrientation;
    QGraphicsScene *orientationScene;

    QGraphicsScene *mapScene;
    QVector<GPSPoint> pointVector;
    QVector<GPSPoint> mapPOI;
    QImage mapImage;
    bool mapTracking;

    QGraphicsScene *vehicleThrustScene;
    int vehicleEnabled;

    QDialog feed1Dialog;
    QDialog feed2Dialog;
    QDialog feed3Dialog;

    QGraphicsView *video1GraphicsView;
    QGraphicsView *video2GraphicsView;
    QGraphicsView *video3GraphicsView;

};

#endif // WIDGET_H
