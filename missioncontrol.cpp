#include "missioncontrol.h"

MissionControl::MissionControl(VehicleConnection *connection)
{
    this->connection = connection;
}

MissionControl::~MissionControl() {
    this->connection = NULL;
}

void MissionControl::setVehicleMode(int mode) {
    /* write to missioncontrolfd */
    QString message;
    qDebug() << "Changing vehicle mode to " << mode;
    message.sprintf("/set_mode %d", mode);

    if(this->connection->writeToVehicle(message))
        emit(vehicleModeChanged(mode));
    else
        qDebug() << "Failed to change vehicle mode!";

    this->connection->writeToVehicle("/set_thrust 0,0,0,0");
}

void MissionControl::setPIDValues(float kp, float ki, float kd) {
    QString message;
    message.sprintf("/set_pid_values %f,%f,%f", kp, ki, kd);
    this->connection->writeToVehicle(message);
}

void MissionControl::updateCurrentOrientation() {
    this->connection->writeToVehicle("/set_current_orientation");
}
