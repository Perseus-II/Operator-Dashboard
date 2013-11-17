#ifndef ORIENTATIONGRAPHIC_H
#define ORIENTATIONGRAPHIC_H

#include <QGraphicsItem>
#include <QDebug>
#include <QPainter>
#include <QRectF>

class OrientationGraphic : public QObject, public QGraphicsItem
{
    Q_OBJECT

private:
    float pitch;
    float roll;
    float yaw;

public slots:
    void updateState(float,float,float);

public:
    OrientationGraphic();
    void change_pitch(float);
    void change_roll(float);
    void change_yaw(float);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // ORIENTATIONGRAPHIC_H
