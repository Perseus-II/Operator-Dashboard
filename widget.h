#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "videofeed.h"
#include <QPixmap>
#include <QDebug>
#include <QGraphicsScene>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_connectButton_clicked();
    void on_rescanButton_clicked();
    void updateVideo1Pixmap(QGraphicsScene *scene);
    void on_startVideo1FeedButton_clicked();

private:
    Ui::Widget *ui;
    VideoFeed video1thread;
    int video1IsRunning;
};

#endif // WIDGET_H
