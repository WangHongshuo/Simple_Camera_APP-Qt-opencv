#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ImageWidget.h"
#include <QCameraInfo>
#include <QTimer>
#include <QTime>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include "wearsantahat.h"

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

    void on_putOnSantaHatCheckBox_stateChanged(int arg1);

    void on_greenHatCheckBox_stateChanged(int arg1);

    void on_greenHatCheckBox_clicked(bool checked);

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
    bool isPutOnSentaHat = false;
    WearSantaHat *santaHat = new WearSantaHat;

};

#endif // MAINWINDOW_H
