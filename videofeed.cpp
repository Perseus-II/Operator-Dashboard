#include "videofeed.h"

void VideoFeed::run()
{
    //qDebug() << "Running video feed" <<QThread::currentThreadId();
    QGraphicsScene *video1Scene = new QGraphicsScene;
    QPixmap video1Pixmap;
    video1Pixmap.load("/Users/ethanhayon/Dropbox/Camera\ Uploads/2013-10-25 16.05.43.jpg");
    video1Pixmap = video1Pixmap.scaledToWidth(300);
    video1Scene->addPixmap(video1Pixmap);

    /* send the latest image to the graphicsview */
    emit renderedImage(video1Scene);
}

void VideoFeed::stop()
{
    /* free up any opencv stuff */
    exit(0);
}
