#ifndef VIDEOFEED_H
#define VIDEOFEED_H

#include <QThread>
#include <QDebug>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

//#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cxcore.h"
#include "czmq.h"

class VideoFeed : public QThread
{
       Q_OBJECT

public:
    void stop();
    void set_ip(QString);
    void set_port(int);


signals:
    void renderedImage(QGraphicsScene *scene);

private:
    void run();
    int stop_loop = false;
    QString ip = "192.168.10.90";
    int port = 9997;
};

#endif // VIDEOFEED_H
