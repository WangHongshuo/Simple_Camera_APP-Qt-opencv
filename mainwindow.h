#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ImageWidget.h"
#include <QCameraInfo>
#include <QTimer>
#include <QTime>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_openCameraButton_clicked();

    void on_closeCameraButton_clicked();

    void on_updateCameraInfoButton_clicked();

    void showCameraFrames();

    void on_selectCamera_activated(int index);

private:
    int cameraCount();
    void setCamerasInfoInComboBox();
    void startGrabFrame();
    void stopGrabFrame();
    int availableCameraCount = 0;
    int cameraIndex;
    QList<QCameraInfo> camerasInfo;
    Ui::MainWindow *ui;
    Mat cameraSteamFrame;
    QImage tempQImage;
    VideoCapture cameraDevices;
    QTimer *timer;
    bool isCameraDeviceOpen = false;
};

#endif // MAINWINDOW_H
