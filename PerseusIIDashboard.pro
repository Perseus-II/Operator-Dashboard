#-------------------------------------------------
#
# Project created by QtCreator 2013-10-30T12:06:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = PerseusIIDashboard
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    videofeed.cpp

HEADERS  += widget.h \
    videofeed.h

FORMS    += widget.ui

LIBS += /usr/local/lib/libopencv_calib3d.dylib /usr/local/lib/libopencv_contrib.dylib /usr/local/lib/libopencv_core.dylib /usr/local/lib/libopencv_features2d.dylib /usr/local/lib/libopencv_flann.dylib /usr/local/lib/libopencv_gpu.dylib /usr/local/lib/libopencv_highgui.dylib /usr/local/lib/libopencv_imgproc.dylib /usr/local/lib/libopencv_legacy.dylib /usr/local/lib/libopencv_ml.dylib /usr/local/lib/libopencv_nonfree.dylib /usr/local/lib/libopencv_objdetect.dylib /usr/local/lib/libopencv_photo.dylib /usr/local/lib/libopencv_stitching.dylib /usr/local/lib/libopencv_ts.dylib /usr/local/lib/libopencv_video.dylib /usr/local/lib/libopencv_videostab.dylib
INCLUDEPATH += = /usr/local/include
INCLUDEPATH += = /usr/local/include/opencv
