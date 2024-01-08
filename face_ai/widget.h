#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWidget>
#include <opencv2/opencv.hpp>
#include "iostream"
#include <math.h>
#include <opencv2/face.hpp>
#include <vector>

#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTimerEvent>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
using namespace  cv;
using namespace cv::face;
using namespace std;

namespace Ui {
class widget;
}

class widget : public QWidget
{
    Q_OBJECT

public:
    explicit widget(QWidget *parent = 0);
    ~widget();
    void on_openCameraBtn_clicked();

    void on_closeCameraBtn_clicked();

    void on_inputFaceBtn_clicked();

private:
    Ui::widget *ui;
    /***************************第一模块：关于摄像头的相关组件*****************************/
    VideoCapture v;            //视频流对象

    Mat src;                   //原图像
    Mat rgb;                   //存放rgb图像，因为qt能识别的图像色彩空间为rgb
    Mat gray;                  //灰度图
    Mat dst;                   //均衡化图像

    CascadeClassifier c;       //级联分类器
    vector<Rect> faces;        //存储人脸举行的容器
    int cameraId;              //摄像头的定时器
    void timerEvent(QTimerEvent *event);   //定时器事件处理函数
    /***************************第二模块：录入人脸的相关组件*****************************/
    Ptr<FaceRecognizer> recognizer;           //人脸识别器
    vector<Mat> study_face;                   //要录入的人脸容器
    vector<int> study_lab;                    //要录入的人脸的标签
    int studyId;                              //人脸录入的定时器
    int flag;                                 //表示是否正在录入人脸
    int count;                                //记录学习的次数
    /***************************第三模块：人脸检测的相关组件*****************************/
    int checkId;                   //人脸检测的定时器
};

#endif // WIDGET_H

