#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ImageWidget-Qt/ImageWidget.h"
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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openCameraButton_clicked();
    void on_closeCameraButton_clicked();
    void on_updateCameraInfoButton_clicked();
    void showCameraFrames();
    void on_selectCamera_activated(int index);
    void on_putOnSantaHatCheckBox_stateChanged(int arg1);
    void on_greenHatCheckBox_clicked(bool checked);
    void showFpsValue();

private:
    int cameraCount();
    void setCamerasInfoInComboBox();
    void startGrabFrame();
    void stopGrabFrame();
    template<class V>
    void setFPSValue(V fpsValue);
    int availableCameraCount = 0;
    int cameraIndex;
    QList<QCameraInfo> camerasInfo;
    Ui::MainWindow *ui;
    Mat cameraSteamFrame;
    QImage tempQImage;
    VideoCapture cameraDevices;
    QTimer *timer;
    QTimer *fpsTimer;
    int fps = 0;
    bool isCameraDeviceOpen = false;
    bool isPutOnSentaHat = false;
    WearSantaHat *santaHat = new WearSantaHat;

};

#endif // MAINWINDOW_H
