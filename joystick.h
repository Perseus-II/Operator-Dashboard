#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QDebug>
#include "SDL.h"

class Joystick : public QObject
{
    Q_OBJECT

public:
    explicit Joystick();
    ~Joystick();


signals:
        void availableJoysticks(QString**);

public slots:

private:

};

#endif // JOYSTICK_H
