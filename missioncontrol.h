#ifndef MISSIONCONTROL_H
#define MISSIONCONTROL_H

#include "vehicleconnection.h"

class MissionControl : public QObject
{
    Q_OBJECT

public:
    MissionControl(VehicleConnection*);
    ~MissionControl();

public slots:
    void setVehicleMode(int mode);
    void setPIDValues(float,float,float);
    void updateCurrentOrientation();

signals:
    void vehicleModeChanged(int);

private:
    VehicleConnection *connection;


};

#endif // MISSIONCONTROL_H
