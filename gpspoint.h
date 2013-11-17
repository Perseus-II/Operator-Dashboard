#ifndef GPSPOINT_H
#define GPSPOINT_H

#include <QPointF>
#include <QDateTime>

class GPSPoint
{

public:
    GPSPoint();
    GPSPoint(QPointF, QDateTime);
    ~GPSPoint();

public:
    QDateTime dateTime;
    QPointF point;
};

#endif // GPSPOINT_H
