#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCameraInfo>
#include <QDebug>
#include <QTime>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <QMessageBox>
#include "mat_qimage_convert.h"
#include "wearsantahat.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->showCameraImage->only_show_image(true);
    ui->closeCameraButton->setEnabled(false);
    availableCameraCount = cameraCount();
    setCamerasInfoInComboBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openCameraButton_clicked()
{
    availableCameraCount = cameraCount();
    if(availableCameraCount)
    {
        cameraIndex = ui->selectCamera->currentIndex();
        if(cameraDevices.isOpened())
            cameraDevices.release();
        cameraDevices.open(cameraIndex);
        if(!cameraDevices.isOpened())
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Can't open camera !"));
            msgBox.exec();
        }
        else
        {
            isCameraDeviceOpen = true;
            startGrabFrame();
            ui->openCameraButton->setEnabled(false);
            ui->closeCameraButton->setEnabled(true);
        }
    }
}

void MainWindow::on_closeCameraButton_clicked()
{
    stopGrabFrame();
    cameraDevices.release();
    isCameraDeviceOpen = false;
    ui->showCameraImage->clear();
    ui->openCameraButton->setEnabled(true);
    ui->closeCameraButton->setEnabled(false);
}

int MainWindow::cameraCount()
{
    if(camerasInfo.length() > 0)
        camerasInfo.clear();
    camerasInfo = QCameraInfo::availableCameras();
    return camerasInfo.length();
}

void MainWindow::setCamerasInfoInComboBox()
{
    ui->selectCamera->clear();
    if(availableCameraCount <= 0)
        ui->selectCamera->addItem("No Camera !");
    else
    {
        for(int i=0;i<availableCameraCount;i++)
            ui->selectCamera->addItem(camerasInfo.at(i).deviceName());
    }
}

void MainWindow::startGrabFrame()
{
    timer = new QTimer(this);
    timer->setInterval(1000/5);
    connect(timer,SIGNAL(timeout()),this,SLOT(showCameraFrames()));
    timer->start();
}

void MainWindow::stopGrabFrame()
{
    timer->stop();
    disconnect(timer,SIGNAL(timeout()),this,SLOT(showCameraFrames()));
    delete timer;
}

void MainWindow::showCameraFrames()
{
    cameraDevices >> cameraSteamFrame;
//    QTime a;
//    a.start();
    santaHat.putOnMySantaHat(cameraSteamFrame);
//    qDebug() << a.elapsed();
    tempQImage = Mat2QImage_with_pointer(santaHat.outputImage);
    ui->showCameraImage->set_image_with_pointer(&tempQImage);
}

void MainWindow::on_updateCameraInfoButton_clicked()
{
    availableCameraCount = cameraCount();
    setCamerasInfoInComboBox();
}

void MainWindow::on_selectCamera_activated(int index)
{
    if(isCameraDeviceOpen)
    {
        cameraDevices.release();
        cameraIndex = index;
        cameraDevices.open(cameraIndex);
        if(!cameraDevices.isOpened())
        {
            timer->stop();
            QMessageBox msgBox;
            msgBox.setText(tr("Can't open camera !"));
            msgBox.exec();
        }
        else
        {
            timer->stop();
            timer->start();
        }
    }
}
