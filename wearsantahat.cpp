#include "wearsantahat.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QTime>
#include <opencv2/highgui.hpp>

WearSantaHat::WearSantaHat()
{
    initializeData();
}

WearSantaHat::~WearSantaHat()
{
    delete [] sentaHat;
}

void WearSantaHat::putOnMySantaHat(Mat &src)
{
    if(!src.data)
    {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Image data is null!"));
        msgBox.exec();
    }
    else if(!isInitializationSuccess)
    {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Initialization Failed!"));
        msgBox.exec();
    }
    else
    {
        inputImage = src;
        mainTask(inputImage);
    }
}

void WearSantaHat::updateHat(int hat)
{
    if(hat > 6)
        hat = 5;
    hatIndex = hat;
    addHat(inputImage,outputImage,hatIndex);
}

int WearSantaHat::facePositionX(int faceIndex)
{
    if(faceIndex < faceCount)
        return facePositionData.ptr<int>(faceIndex)[0];
    else
        return -1;
}

int WearSantaHat::facePositionY(int faceIndex)
{
    if(faceIndex < faceCount)
        return facePositionData.ptr<int>(faceIndex)[1];
    else
        return -1;
}

int WearSantaHat::faceWidth(int faceIndex)
{
    if(faceIndex < faceCount)
        return facePositionData.ptr<int>(faceIndex)[2];
    else
        return -1;
}

int WearSantaHat::faceHeight(int faceIndex)
{
    if(faceIndex < faceCount)
        return facePositionData.ptr<int>(faceIndex)[3];
    else
        return -1;
}

void WearSantaHat::selectHat(int index)
{
    hatIndex = index;
}

void WearSantaHat::mainTask(Mat &src)
{
    if(src.channels() == 3)
        cvtColor(src,grayImage,CV_RGB2GRAY);
    else if(src.channels() == 4)
        cvtColor(src,grayImage,CV_RGBA2BGRA);
    else
        grayImage = src.clone();
    detecteFace(grayImage,facePositionData);
    addHat(inputImage,outputImage,hatIndex);
}

void WearSantaHat::initializeData()
{
    QString Qpath = QCoreApplication::applicationDirPath();
    String path;
    Qpath += "/haarcascade_frontalface_alt2.xml";
    path = Qpath.toStdString();
    faceDetecter.load(path);
    if(faceDetecter.empty())
    {
        isInitializationSuccess = false;
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Can't load detecer data!"));
        msgBox.exec();
    }
    Qpath = QCoreApplication::applicationDirPath();
    QString temp;
    for(int i=0;i<6;i++)
    {
        temp = Qpath + "/hat/" + QString::number(i+1) + ".png";
        path = temp.toStdString();
        sentaHat[i] = imread(path,IMREAD_UNCHANGED);
        if(!sentaHat[i].data)
        {
            isInitializationSuccess = false;
            QMessageBox msgBox;
            msgBox.setText(QObject::tr("Can't load hat!"));
            msgBox.exec();
            break;
        }
    }
}

void WearSantaHat::detecteFace(Mat &src,Mat &facePositionData)
{
//    QTime a;
//    a.start();
    std::vector<Rect> faces;
    faceDetecter.detectMultiScale(src,faces,1.2,3,0,Size(0,0));
//    qDebug() << a.elapsed();
    faceCount = static_cast<int>(faces.size());
    if(faces.size()>0)
    {
        facePositionData = Mat(faces.size(),4,CV_32SC1);
        for(int i=0; i<static_cast<int>(faces.size()); i++)
        {
//            rectangle(dst, Point(faces[i].x, faces[i].y),
//                      Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height),
//                      Scalar(0, 0, 255, 255),2,8);
            facePositionData.ptr<int>(i)[0] = faces[i].x;
            facePositionData.ptr<int>(i)[1] = faces[i].y;
            facePositionData.ptr<int>(i)[2] = faces[i].width;
            facePositionData.ptr<int>(i)[3] = faces[i].height;
        }
    }
}

void WearSantaHat::addHat(Mat &src, Mat &dst, int hatIndex)
{
    int x, y;
    dst = src.clone();
    if(faceCount > 0)
    {
        Mat hat = sentaHat[hatIndex].clone();
        for(int i=0; i<faceCount; i++)
        {
            resize(hat,hat,Size(faceWidth(i),faceWidth(i)),INTER_CUBIC);

            Mat BGRAChannels[4];
            split(hat,BGRAChannels);
            Mat hatMask = BGRAChannels[3];
            // hat是4通道图片，如果输入图片为3或1通道，转换一下
            if(src.channels() == 3)
            {
                QTime a;
                a.start();
                Mat temp[3] = {BGRAChannels[0],BGRAChannels[1],BGRAChannels[2]};
                merge(temp,3,hat);
                qDebug() << a.elapsed();
            }
            else if (src.channels() == 1)
            {
                Mat temp[4] = {src, src, src, src};
                merge(temp,4,dst);
            }
            // 防止越界
            x = facePositionX(i);
            y = facePositionY(i) - hat.rows;
            if(y < 0)
            {
                Mat hatROI = hat(Rect(0,-y,hat.cols,hat.rows+y));
                Mat hatMaskROI = hatMask(Rect(0,-y,hat.cols,hat.rows+y));
                Mat imageROI = dst(Rect(x,0,hat.cols,hat.rows+y));
                hatROI.copyTo(imageROI,hatMaskROI);
            }
            else
            {
                Mat imageROI = dst(Rect(x,y,hat.cols,hat.rows));
                hat.copyTo(imageROI,hatMask);
            }
        }
    }
}

