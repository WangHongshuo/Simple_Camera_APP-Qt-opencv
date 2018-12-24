#include "wearsantahat.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QTime>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include "facedetect-dll.h"

WearSantaHat::WearSantaHat()
{
    initializeData();
}

WearSantaHat::~WearSantaHat()
{
    delete [] sentaHat;
    delete [] faceData;
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

void WearSantaHat::putOnGreenHat(bool greenHat)
{
    if(greenHat)
        isPutOnGreenHat = true;
    else
        isPutOnGreenHat = false;
}

void WearSantaHat::mainTask(Mat &src)
{
    if(src.channels() == 3)
        cvtColor(src,grayImage,COLOR_RGB2GRAY);
    else if(src.channels() == 4)
        cvtColor(src,grayImage,COLOR_RGBA2BGRA);
    else
        grayImage = src.clone();
    detecteFace(grayImage,facePositionData);
    addHat(inputImage,outputImage,hatIndex);
}

void WearSantaHat::initializeData()
{
    QString Qpath = QCoreApplication::applicationDirPath();
    String path;
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
    faceDetectResult = facedetect_frontal_surveillance(faceData, (unsigned char*)src.data,
                                          src.cols, src.rows, (int)src.step,
                                          1.2f, 2, 48, 0 ,1);
//    qDebug() << a.elapsed();
    faceCount = *faceDetectResult;
    if(faceCount>0)
    {
        facePositionData = Mat(faceCount,4,CV_32SC1);
//        qDebug() << "face : " << faceCount;
        for(int i=0; i<faceCount; i++)
        {
            short *p = ((short*)(faceData+4))+142*i;
            facePositionData.ptr<int>(i)[0] = p[0];
            facePositionData.ptr<int>(i)[1] = p[1];
            facePositionData.ptr<int>(i)[2] = p[2];
            facePositionData.ptr<int>(i)[3] = p[3];
//            rectangle(inputImage, Point((int)p[0], (int)p[1]),
//                      Point((int)p[0] + (int)p[3], (int)p[1] + (int)p[3]),
//                      Scalar(0, 0, 255, 255),2,8);
//            qDebug() << facePositionData.ptr<int>(i)[0]
//            <<facePositionData.ptr<int>(i)[1]
//            <<facePositionData.ptr<int>(i)[2]
//            <<facePositionData.ptr<int>(i)[3];
        }
    }
}

void WearSantaHat::addHat(Mat &src, Mat &dst, int hatIndex)
{
    int srcX1, srcY1, srcX2, srcY2;
    dst = src.clone();
    if(faceCount > 0)
    {
        for(int i=0; i<faceCount; i++)
        {
            Mat hat = sentaHat[hatIndex].clone();
            int hatSize = int(faceWidth(i)*hatScale[hatIndex]);
            resize(hat,hat,Size(hatSize,hatSize),INTER_CUBIC);
            int hatX1 = 0, hatY1 = 0, hatX2 = hatSize-1, hatY2 = hatSize-1;

            Mat BGRAChannels[4];
            split(hat,BGRAChannels);
            Mat hatMask = BGRAChannels[3];
            // 绿化
            if(isPutOnGreenHat)
            {
                Mat temp[4] = {BGRAChannels[0],BGRAChannels[2],BGRAChannels[1],BGRAChannels[3]};
                merge(temp,4,hat);
                split(hat,BGRAChannels);
            }

            // hat是4通道图片，如果输入图片为3或1通道，转换一下
            if(src.channels() == 3)
            {
                Mat temp[3] = {BGRAChannels[0],BGRAChannels[1],BGRAChannels[2]};
                merge(temp,3,hat);
            }
            else if (src.channels() == 1)
            {
                Mat temp[4] = {src, src, src, src};
                merge(temp,4,dst);
            }

            srcX1 = facePositionX(i) - int((0.5*(hatScale[hatIndex]-1.0f))*faceWidth(i));
            srcY1 = facePositionY(i) - int(hat.rows*hatYScale[hatIndex]);
            srcX2 = srcX1 + hatSize - 1;
            srcY2 = srcY1 + hatSize - 1;
            // 防止越界
            if(srcX1 < 0)
            {
                hatX1 = -srcX1;
                srcX1 = 0;
            }
            if(srcY1 < 0)
            {
                hatY1 = -srcY1;
                srcY1 = 0;
            }
            if(srcX2 > src.cols-1)
            {
                hatX2 = hatX2-(srcX2-src.cols+1);
                srcX2 = src.cols-1;
            }
            if(srcY2 > src.rows-1)
            {
                hatY2 = hatY2-(srcY2-src.rows+1);
                srcY2 = src.rows-1;
            }

            Mat imageROI = dst(Rect(srcX1,srcY1,srcX2-srcX1+1,srcY2-srcY1+1));
            Mat hatROI = hat(Rect(hatX1,hatY1,hatX2-hatX1+1,hatY2-hatY1+1));
            Mat hatMaskROI = hatMask(Rect(hatX1,hatY1,hatX2-hatX1+1,hatY2-hatY1+1));
            hatROI.copyTo(imageROI,hatMaskROI);
        }
    }
}

