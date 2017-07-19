#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <QSlider>
#include <QSpinBox>
#include <QSize>

QImage  Mat2QImage(cv::Mat cvImg);

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

    void nextFrame();
    void show_open_img(cv::Mat &input);
    void on_open_cam_clicked();
    void on_close_cam_clicked();
    void on_open_img_clicked();

    void on_ui_close_clicked();

    void on_grayscale_clicked(bool checked);

    void on_edge_clicked(bool checked);

    void on_slider_th1_valueChanged();

    void on_slider_th2_valueChanged();

private:

    Ui::MainWindow *ui;
    cv::Mat frame;
    cv::Mat gray_img;
    cv::Mat canny_img;
    cv::Mat input_img;
    cv::VideoCapture capture;
    QImage image;
    QTimer *timer;
    int th1;
    int th2;
    bool is_open_img = false;
    bool is_gray_img = false;
    bool is_canny_img = false;
};

#endif // MAINWINDOW_H
