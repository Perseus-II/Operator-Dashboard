#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QDebug>
#include <QThread>
#include "SDL.h"

class Joystick : public QThread
{
    Q_OBJECT

public:
    bool attachJoystick(int);



signals:
    void availableJoysticks(QString**);

public slots:

private:
    void run();
    SDL_Joystick *stick = NULL;
    SDL_Event event;

};

#endif // JOYSTICK_H
