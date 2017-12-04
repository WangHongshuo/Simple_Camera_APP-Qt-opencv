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

# xp支持

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01

DEFINES += _ATL_XP_TARGETING
QMAKE_CFLAGS += /D_USING_V140_SDK71_
QMAKE_CXXFLAGS += /D_USING_V140_SDK71_
LIBS += -L$$quote(C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib)
INCLUDEPATH += $$quote(C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include)

# MinGW32

#INCLUDEPATH += D:/opencv3.3.1/opencv_use/include\
#               D:/opencv3.3.1/opencv_use/include/opencv\
#               D:/opencv3.3.1/opencv_use/include/opencv2
#LIBS += -L D:/opencv3.3.1/opencv_use/lib/libopencv_*.a

# MSVC relaese

INCLUDEPATH += D:/opencv3.0/build/include\
               D:/opencv3.0/build/include/opencv\
               D:/opencv3.0/build/include/opencv2
LIBS += D:/opencv3.0/build/x86/vc12/lib/opencv_ts300.lib
LIBS += D:/opencv3.0/build/x86/vc12/lib/opencv_world300.lib

