#include "widget.h"
#include "ui_widget.h"
#include <map>

widget::widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    ui->setupUi(this);

    //将登录按钮设置成不可用
    ui->loginBtn->setEnabled(false);
    //启动摄像头
    if(!v.open(0))
    {
        QMessageBox::information(this,"错误","打开摄像头失败");
        return;
    }
    connect(ui->cameraOpen,&QPushButton::clicked,this,&widget::on_openCameraBtn_clicked);
    connect(ui->cameraClose,&QPushButton::clicked,this,&widget::on_closeCameraBtn_clicked);
    connect(ui->inputFaceBtn,&QPushButton::clicked,this,&widget::on_inputFaceBtn_clicked);

    //将级联分类器加载进来
    if(!c.load("D:\\opencv\\opencv_4.8.0\\opencv\\sources\\data\\haarcascades_cuda\\haarcascade_frontalface_alt.xml"))
    {
        QMessageBox::information(this,"失败","人脸识别模型装载失败");
        return;
    }

    //配置人脸识别器
    QFile file("D:\\face\\myFace.xml");
    //判断文件是否存在，如果存在，则直接下载，如果不存在，则创建一个人脸识别器
    if(file.exists())
    {
        //人脸模型存在，直接下载即可
        recognizer = FaceRecognizer::load<LBPHFaceRecognizer>("D:\\face\\myFace.xml");
    }
    else
    {
        //人脸模型不存在，则需要创建
        recognizer = LBPHFaceRecognizer::create();
    }

    //启动人脸检测到的定时器
    checkId = this->startTimer(3000);

    //设置人脸识别的可信度
    recognizer->setThreshold(100);
    flag = 0;          //表明开始时就处于检测
}

widget::~widget()
{
    delete ui;
}

//打开摄像头按钮对应的槽函数
void widget::on_openCameraBtn_clicked()
{
    //启动定时器
    cameraId = this->startTimer(20);
    ui->cameraLab->show();
}

//关闭摄像头按钮对应的槽函数
void widget::on_closeCameraBtn_clicked()
{
    //关闭定时器
    this->killTimer(cameraId);
    ui->cameraLab->hide();
}

//定时器事件处理函数
void widget::timerEvent(QTimerEvent *event)
{
    //判断是哪个定时器到位
    if(event->timerId() == cameraId)
    {
        //1.从摄像头中读取一张图像
        v.read(src);      //得到原图

        //2.将图像翻转
        flip(src,src,1);

        //3.将src的bgr图像转换为rgb图像
        cvtColor(src,rgb,COLOR_BGR2RGB);

        //4.重新设置大小
        cv::resize(rgb,rgb,Size(300,300));

        //5.灰度处理
        cvtColor(rgb,gray,COLOR_RGB2GRAY);

        //6.均衡化处理
        equalizeHist(gray,dst);

        //7.使用级联分类器获取人脸矩形区域
        c.detectMultiScale(dst,faces);

        //8.将矩形框绘制到rgb图像上
        for(int i=0; i<faces.size(); i++)
        {
            rectangle(rgb,faces[i],Scalar(255,0,0),2);
        }

        //9.使用rgb图像，将Mat图，构造出一个qt能识别的图像
        QImage img(rgb.data,rgb.cols,rgb.rows,rgb.cols*rgb.channels(),QImage::Format_RGB888);
        //功能：通过其他图像构造出一个QImage图像
        //参数1：其他图像的数据
        //参数2：图像的宽度
        //参数3：图像的高度
        //参数4：每一行的字节数
        //参数5：图像格式，24位图，每一种颜色使用8位表示

        //10.将图像展示到lab中
        ui->cameraLab->setPixmap(QPixmap::fromImage(img));


    }

    //判断是否是人脸录入定时器到位
    if(event->timerId() == studyId)
    {
        //判断ui界面是否有矩形框
        if(faces.empty())return;

        //判断人脸识别器是否存在
        if(recognizer.empty())return;

        //提示正在录入人脸
        qDebug() << "正在录入人脸...";

        //获取ui界面中矩形框框起来的人脸区域
        Mat face = src(faces[0]);

        //将该图像进行重新设置大小
        cv::resize(face,face,Size(100,100));

        //灰度处理
        cvtColor(face,face,COLOR_BGR2GRAY);

        //均衡化处理
        equalizeHist(face,face);

        //将人脸放入学习容器中
        study_face.push_back(face);
        study_lab.push_back(1);
        count++;                     //表明完成一次人脸的存放
        if(count == 2)              //已经收集50张人脸进行学习
        {
            count = 0;               //以便于下一次录入

            //更新人脸模型
            //函数原型：CV_WRAP virtual void update(InputArrayOfArrays src, InputArray labels);
            //参数1：要进行更新的人脸数组
            //参数2：要更新的人脸标签数组
            //返回值：无
            recognizer->update(study_face,study_lab);

            //将数据模型保存到本地磁盘中
            recognizer->save("D:\\face\\myFace.xml");

            //殿后工作
            study_face.clear();       //清空人脸数组
            study_lab.clear();        //清空标签数组
            flag = 0;                 //表明录入已经结束，可以进行人脸检测了
            ui->inputFaceBtn->setEnabled(true);    //按钮设置成可用状态
            this->killTimer(studyId);              //关闭人脸录入的定时器
            QMessageBox::information(this,"成功","人脸录入成功");
        }
    }

    //判断是否是人脸检测的定时器到位
    if(event->timerId() == checkId)
    {
        qDebug() << "正在检测...";

        //判断是否处于检测
        if(flag == 0)
        {
            QFile file("D:\\face\\myFace.xml");
            if(file.exists())              //表明人脸模型存在的基础上进行识别
            {
                if(faces.empty() || recognizer.empty()) return;              //ui界面无矩形框或者没有人脸识别器

                //到此表明可以进行检测
                Mat face = src(faces[0]);

                //重新设置大小，保持跟保存人脸时一致
                cv::resize(face,face,Size(100,100));

                //灰度处理
                cvtColor(face,face,COLOR_BGR2GRAY);

                //均衡化处理
                equalizeHist(face,face);

                //定义记录检测后返回的结果的变量
                int lab = -1;                //返回的图像的标签
                double conf = 0.0;           //返回图像的可信度

                //将该人脸进行预测
                recognizer->predict(face,lab,conf);
                qDebug() << "lab = " << lab << "  conf = " << conf;

                //对人脸识别后的结果进行判断
                if(lab != -1)
                {
                    ui->loginBtn->setEnabled(true);
                }
            }
        }
    }

}

//录入人脸按钮对应的槽函数
void widget::on_inputFaceBtn_clicked()
{
    //启动人脸录入的定时器
    qDebug() << "开始进行人脸录入...";

    studyId = this->startTimer(60);

    //将按钮设置成不可用状态
    ui->inputFaceBtn->setEnabled(false);
    //将flag设置成1，表示正在录入人脸，不要进行人脸检测了
    flag = 1;

    count = 0;       //清空计数器
}


