#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QDebug>
#include <QThread>
#include "SDL.h"
#include "vehicleconnection.h"

class Joystick : public QObject
{
    Q_OBJECT

public:
    Joystick(VehicleConnection*);
    ~Joystick();
    bool attached;
    void sleep(int ms) { QThread::sleep(ms); }



signals:
    void joystickStateChanged(bool);
    void thrustVectorChanged(float,float,float,float);

public slots:
    void attachJoystick();
    void reattachJoystick();
    void detachJoystick();



private:
    SDL_Joystick *stick;
    VehicleConnection *vehicleConnection;

};

#endif // JOYSTICK_H
