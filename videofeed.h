#ifndef VIDEOFEED_H
#define VIDEOFEED_H

#include <QThread>
#include <QDebug>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

class VideoFeed : public QThread
{
       Q_OBJECT

public:
    void stop();

signals:
    void renderedImage(QGraphicsScene *scene);

private:
    void run();
    int stop_loop = false;
};

#endif // VIDEOFEED_H
