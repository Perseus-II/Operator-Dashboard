#include "orientationgraphic.h"

OrientationGraphic::OrientationGraphic()
{
}

QRectF OrientationGraphic::boundingRect() const
{
    return QRectF(0,0,100,100);

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
    yaw = y;
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

    setTransformOriginPoint(rec.center());
    setRotation(yaw);
    painter->fillRect(rec, brush);
    painter->drawRect(rec);

}

