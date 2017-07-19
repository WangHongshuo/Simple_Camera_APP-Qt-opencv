#-------------------------------------------------
#
# Project created by QtCreator 2016-11-17T16:51:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoCameraApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += D:/opencv_use/include\
               D:/opencv_use/include/opencv\
               D:/opencv_use/include/opencv2
LIBS += -L D:/opencv_use/lib/libopencv_*.a
