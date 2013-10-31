#include "videofeed.h"


void VideoFeed::run() {

    //cv::Mat cameraFrame;
    cv::Mat rgbFrame;
    cv::Mat cameraFrame;
    CvMat q;


    // ZeroMQ
    zctx_t *ctx = zctx_new();
    void *subscriber = zsocket_new(ctx, ZMQ_SUB);
    zmsg_t *msg;
    zframe_t *frame;
    byte *buffer;

    // connect to publisher on the BBB
    zmq_connect(subscriber, "tcp://192.168.10.90:5563");
    // we subscribe to all messages
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);


    while(true) {
        QGraphicsScene *video1Scene = new QGraphicsScene;

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

        cameraFrame = cvDecodeImageM(&q, 1);

        switch (cameraFrame.type()) {
            case CV_8UC1:
                cvtColor(cameraFrame, rgbFrame, CV_GRAY2RGB);
                break;
            case CV_8UC3:
                cvtColor(cameraFrame, rgbFrame, CV_BGR2RGB);
                break;
        }

        QPixmap video1Pixmap;
        video1Pixmap = QPixmap::fromImage(QImage((unsigned char*) rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step1(), QImage::Format_RGB888));
        video1Scene->addPixmap(video1Pixmap);

        // destroy the message
        zmsg_destroy (&msg);

        /* send the latest image to the graphicsview */
        emit renderedImage(video1Scene);
        if (cv::waitKey(30) >= 0) break;

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

