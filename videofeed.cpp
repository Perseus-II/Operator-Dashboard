#include "videofeed.h"


void VideoFeed::run()
{
    cv::VideoCapture capture(0);
	if(!capture.isOpened()) {
		qDebug() << "Failed to open camera";
	}
    cv::Mat cameraFrame;
    cv::Mat rgbFrame;
    while(true) {
        if(stop_loop) {
            stop_loop = false;
            break;
        }

        QGraphicsScene *video1Scene = new QGraphicsScene;

        capture.read(cameraFrame);
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

        /* send the latest image to the graphicsview */
        emit renderedImage(video1Scene);
        if (cv::waitKey(30) >= 0) break;
    }
}

void VideoFeed::stop()
{
	/* free up any opencv stuff */
	stop_loop = true;
	qDebug() << "Stopping loop";
}

