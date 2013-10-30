#ifndef VIDEOFEED_H
#define VIDEOFEED_H

#include <QThread>
#include <QDebug>
#include <QGraphicsView>

class VideoFeed : public QThread
{
       Q_OBJECT

public:
    void stop();

signals:
    void renderedImage(QGraphicsScene *scene);

private:
    void run();
};

#endif // VIDEOFEED_H
