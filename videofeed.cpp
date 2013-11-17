#include "videofeed.h"

void VideoFeed::set_ip(QString newip) {
    ip = newip;
}

void VideoFeed::set_port(int newport) {
    port = newport;
}

void VideoFeed::run() {

    //cv::Mat cameraFrame;

    CvMat q;


    // ZeroMQ
    zctx_t *ctx = zctx_new();
    void *subscriber = zsocket_new(ctx, ZMQ_SUB);
    zmsg_t *msg;
    zframe_t *frame;
    byte *buffer;

    // connect to publisher on the BBB
    QString address = QString("tcp://%1:%2")
                         .arg(ip).arg(port);
    //address.sprintf("tcp://%s:%d", ip, port);

    zmq_connect(subscriber, address.toLocal8Bit().data());
    // we subscribe to all messages
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    QGraphicsPixmapItem *pixmapitem = new QGraphicsPixmapItem();
    QGraphicsPixmapItem *prev_pixmapitem = new QGraphicsPixmapItem();

    while(true) {
        cv::Mat rgbFrame;
        cv::Mat cameraFrame;

        if(stop_loop) {
            stop_loop = false;
            break;
        }

        msg = zmsg_recv (subscriber);

        /* skip the first frame, image data is contained in second frame */
        zmsg_next (msg);

        frame = zmsg_next (msg);

        /* incomplete packet, throw the frame away */
        if(frame == NULL)
            continue;

        buffer = zframe_data(frame);

        q = cv::Mat(768, 1024, CV_8UC3, (void*)buffer);

        cameraFrame = cv::imdecode(cv::Mat(&q), 1);
/*        cameraFrame = cvDecodeImageM(&q, 1);
*/

        switch (cameraFrame.type()) {
            case CV_8UC1:
                cvtColor(cameraFrame, rgbFrame, CV_GRAY2RGB);
                break;
            case CV_8UC3:
                cvtColor(cameraFrame, rgbFrame, CV_BGR2RGB);
                break;
        }

        QPixmap pixmap;


        QGraphicsScene *scene = new QGraphicsScene();

        pixmap = QPixmap::fromImage(QImage((unsigned char*) rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step1(), QImage::Format_RGB888));
        pixmap = pixmap.scaledToWidth(651);

        if(prev_pixmapitem)
            delete(prev_pixmapitem);

        prev_pixmapitem = pixmapitem;

        pixmapitem = scene->addPixmap(pixmap);

        // destroy the message
        zmsg_destroy (&msg);



        /* send the latest image to the graphicsview */

        emit renderedImage(scene);

        if (cv::waitKey(10) >= 0) break;

    }
    zmq_close (subscriber);
    zmq_ctx_destroy (ctx);
}

void VideoFeed::stop()
{
	/* free up any opencv stuff */
    stop_loop = true;
	qDebug() << "Stopping loop";
}

