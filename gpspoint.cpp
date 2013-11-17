#include "gpspoint.h"

GPSPoint::GPSPoint()
{

}

GPSPoint::GPSPoint(QPointF point, QDateTime dateTime)
{
    this->point = point;
    this->dateTime = dateTime;
}

GPSPoint::~GPSPoint()
{
    //delete(this->point);
    //delete(this->dateTime);
}
