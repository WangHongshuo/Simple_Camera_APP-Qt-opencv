#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QSlider>
#include <QSpinBox>
#include <QString>
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QSize>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);
    ui->edge->setEnabled(false);
    ui->save_pic->setEnabled(false);
    ui->close_cam->setEnabled(false);

    ui->slider_th1->setMinimum(0);
    ui->slider_th1->setMaximum(255);
    ui->slider_th1->setValue(80);
    ui->slider_th1->setSingleStep(1);

    ui->spinBox_th1->setMinimum(0);
    ui->spinBox_th1->setMaximum(255);
    ui->spinBox_th1->setValue(80);
    ui->spinBox_th1->setSingleStep(1);

    ui->slider_th2->setMinimum(0);
    ui->slider_th2->setMaximum(255);
    ui->slider_th2->setValue(160);
    ui->slider_th2->setSingleStep(1);

    ui->spinBox_th2->setMinimum(0);
    ui->spinBox_th2->setMaximum(255);
    ui->spinBox_th2->setValue(160);
    ui->spinBox_th2->setSingleStep(1);
    connect(ui->slider_th1,SIGNAL(valueChanged(int)),ui->spinBox_th1,SLOT(setValue(int)));
    connect(ui->spinBox_th1,SIGNAL(valueChanged(int)),ui->slider_th1,SLOT(setValue(int)));
    connect(ui->slider_th2,SIGNAL(valueChanged(int)),ui->spinBox_th2,SLOT(setValue(int)));
    connect(ui->spinBox_th2,SIGNAL(valueChanged(int)),ui->slider_th2,SLOT(setValue(int)));

}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::nextFrame()
{
    capture >> frame;
    if(ui->grayscale->isChecked())
    {
        ui->edge->setEnabled(true);
        cv::cvtColor(frame,frame,CV_BGR2GRAY);
    }
    else
    {
        ui->edge->setChecked(false);
        ui->edge->setEnabled(false);
    }
    if(ui->edge->isChecked())
    {
            th1 = ui->slider_th1->value();
            th2 = ui->slider_th2->value();
            cv::Canny(frame,frame,th1,th2);
    }
    if (!frame.empty())
    {
        image = Mat2QImage(frame);
        ui->label->setPixmap(QPixmap::fromImage(image));
    }
}

void MainWindow::on_open_cam_clicked()
{
    if (capture.isOpened())
        capture.release();
    capture.open(0);
    ui->label->clear();
    ui->save_pic->setEnabled(true);
    ui->close_cam->setEnabled(true);
    ui->open_cam->setEnabled(false);
    if (capture.isOpened())
    {
        capture >> frame;
        if (!frame.empty())
        {

            image = Mat2QImage(frame);
            ui->label->setPixmap(QPixmap::fromImage(image));
            timer = new QTimer(this);
            timer->setInterval(1000/30);
            //sender,SIGNAL(signal),receiver,SLOT(slot)
            connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
            timer->start();
        }
    }
}


QImage Mat2QImage(cv::Mat mat)
{
    //单通道
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);

        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    //3通道
    else if(mat.type() == CV_8UC3)
    {
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    //4通道
    else if(mat.type() == CV_8UC4)
    {
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}

void MainWindow::on_close_cam_clicked()
{
    if (capture.isOpened())
    {
        capture.release();
        timer->stop();
        ui->label->clear();
        ui->save_pic->setEnabled(false);
        ui->edge->setChecked(false);
        ui->edge->setEnabled(false);
        ui->open_cam->setEnabled(true);
        ui->close_cam->setEnabled(false);
    }
}


void MainWindow::on_open_img_clicked()
{
    bool flag;
    if(capture.isOpened())
    {
        flag = true;
        capture.release();
        timer->stop();
    }
    else
        flag =false;
    ui->label->clear();
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();
    input_img = cv::imread(name);
    if(!input_img.data)
    {
        ui->save_pic->setEnabled(false);
        if(flag)
        {
            capture.open(0);
            timer->start();
        }
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    }
    else
    {
        ui->grayscale->setChecked(false);
        ui->edge->setChecked(false);
        ui->save_pic->setEnabled(true);
        is_open_img = true;
        ui->open_cam->setEnabled(true);
        ui->close_cam->setEnabled(false);
        show_open_img(input_img);
    }
}

void MainWindow::show_open_img(cv::Mat &input_img)
{
    image = Mat2QImage(input_img);
    if(input_img.cols > 640)
    {
        int h = int(double(input_img.rows)/double(input_img.cols)*double(640));
        QSize img_scale(640,h);
        image = image.scaled(img_scale,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
    if(image.height() > 480)
    {
        int w = int(double(image.width())/double(image.height())*double(480));
        QSize img_scale(w,480);
        image = image.scaled(img_scale,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
    ui->label->setScaledContents(false);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->clear();
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_ui_close_clicked()
{
    QApplication::exit();
}

void MainWindow::on_grayscale_clicked(bool checked)
{
    if(!capture.isOpened())
    {
        if(checked && is_open_img)
        {
            cv::cvtColor(input_img,gray_img,CV_BGR2GRAY);
            is_gray_img = true;
            show_open_img(gray_img);
            ui->edge->setEnabled(true);
        }
        else if(!checked && is_open_img)
        {
            is_gray_img = false;
            if(is_canny_img)
            {
                ui->edge->setChecked(false);
                ui->edge->setEnabled(false);
                show_open_img(input_img);
            }
        }
    }
}

void MainWindow::on_edge_clicked(bool checked)
{
    if(!capture.isOpened())
    {
        if(checked && is_open_img)
        {
            th1 = ui->slider_th1->value();
            th2 = ui->slider_th2->value();
            if(is_gray_img)
            {
                cv::Canny(gray_img,canny_img,th1,th2);
                is_canny_img = true;
                show_open_img(canny_img);
            }
            else
            {
                cv::Canny(input_img,canny_img,th1,th2);
                is_canny_img = true;
                show_open_img(canny_img);
            }
        }
        else if(!checked && is_open_img)
        {
            if(is_gray_img)
                show_open_img(gray_img);
            else
                show_open_img(input_img);
        }
    }
}

void MainWindow::on_slider_th1_valueChanged()
{
    if(is_canny_img)
    {
        MainWindow::on_edge_clicked(1);
    }
}

void MainWindow::on_slider_th2_valueChanged()
{
    if(is_canny_img)
    {
        MainWindow::on_edge_clicked(1);
    }
}


void MainWindow::on_save_pic_clicked()
{

    QImage temp;
    temp = image;
    QString filename = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                    "C:/",
                                                    tr("Images (*.png *.xpm *.jpg *.tiff *.bmp)"));

    temp.save(filename);
}
