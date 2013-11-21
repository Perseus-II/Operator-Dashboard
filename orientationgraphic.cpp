#include "orientationgraphic.h"

OrientationGraphic::OrientationGraphic()
{
}

QRectF OrientationGraphic::boundingRect() const
{
    return QRectF(0,0,200,200);

}

void OrientationGraphic::change_pitch(float p) {
    pitch = p;
    update();
}

void OrientationGraphic::change_roll(float r) {
    roll = r;
    update();
}

void OrientationGraphic::change_yaw(float y) {
    yaw = -1*(y - 77);
    update();
}

void OrientationGraphic::updateState(float pitch, float roll, float yaw) {
    change_pitch(pitch);
    change_roll(roll);
    change_yaw(yaw);
    qDebug() << "Updating graphics: " << yaw;
}

void OrientationGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();

    QBrush brush(Qt::blue);

    setRotation(yaw);
    setTransformOriginPoint(100,100);
    painter->setBrush(brush);
    //painter->fillRect(rec, brush);
    //painter->drawLine(100,100,100,200);
    painter->drawLine(100,100,100,30);

    //painter->drawRect(rec);

}

