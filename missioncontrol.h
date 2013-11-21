#ifndef MISSIONCONTROL_H
#define MISSIONCONTROL_H

#include "vehicleconnection.h"

class MissionControl : public QObject
{
    Q_OBJECT

public:
    MissionControl(VehicleConnection*);
    ~MissionControl();
    int mode;

public slots:
    void setVehicleMode(int mode);
    void setPIDValues(float,float,float);
    void updateCurrentOrientation();
    void updateDesiredDepth(float);

signals:
    void vehicleModeChanged(int);

private:
    VehicleConnection *connection;


};

#endif // MISSIONCONTROL_H
