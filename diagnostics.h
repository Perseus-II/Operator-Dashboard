#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <QObject>
#include <QDebug>
#include "vehicleconnection.h"
#include <stdlib.h>
#include <unistd.h>
#include <QPointF>

class Diagnostics : public QObject
{
    Q_OBJECT

public:
    Diagnostics(VehicleConnection*);
    ~Diagnostics();

public slots:
    void process();

signals:
    void infoUpdated(float, float, float);
    void pidValuesUpdated(float, float, float);
    void newMapPointAvailable(QPointF);
    void thrustVectorChanged(float,float,float,float);

private:
    VehicleConnection *connection;
};

#endif // DIAGNOSTICS_H
