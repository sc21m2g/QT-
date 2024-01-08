#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QSplitter>
#include<QFileDialog>
#include<QInputDialog>
#include<QPushButton>
#include<QDebug>
#include<QCheckBox>
#include<QVBoxLayout>
#include<QLabel>
#include<QTimer>
#include<opencv2/opencv.hpp>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlError>
#include<QtSql/QSqlQuery>
#include "widget.h"

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainwindow)
{
    ui->setupUi(this);
    setWindowTitle("opencv4 DNN 人脸检测与识别演示");
    QSplitter*splitter = new QSplitter();
    QWidget *btnPannel = new QWidget();
    QWidget *canvasPanel = new QWidget();
    QPushButton *selectImgBtn = new QPushButton("选择图象/视频...");
    QPushButton *registerFaceBtn = new QPushButton("人脸识别");
    QPushButton *saveBtn = new QPushButton("人脸保存");
    timer=new QTimer(this);
    this->matchFaceOption = new QCheckBox("识别人脸");
    this->showlandmarkOption = new QCheckBox("显示landmark");
    this->showFPSOption = new QCheckBox("显示FPS");
    QVBoxLayout *vb1= new QVBoxLayout();
    vb1->addWidget(selectImgBtn);
    vb1->addWidget(registerFaceBtn);
    vb1->addWidget(saveBtn);
    vb1->addWidget(this->matchFaceOption);
    vb1->addWidget(this->showlandmarkOption);
    vb1->addWidget(this->showFPSOption);
    vb1->addStretch(1);
    canvas= new QLabel();
    QVBoxLayout* vb2 = new QVBoxLayout();
    vb2->addWidget(this->canvas);
    canvasPanel->setLayout(vb2);
    btnPannel->setLayout(vb1);
    splitter->addWidget(btnPannel);
    splitter->addWidget(canvasPanel);
    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,9);
    QHBoxLayout *layout= new QHBoxLayout(ui->centralwidget);
    layout ->addWidget(splitter);
    connect(selectImgBtn, &QPushButton::clicked, this, &mainwindow::load_image);
    connect(registerFaceBtn, &QPushButton::clicked, this, &mainwindow::register_face);
    connect(saveBtn,&QPushButton::clicked,this,&mainwindow::saveWindow);

    std::string face_detetor_path = "D:\\QTwork\\face\\face_detection_yunet_2023mar.onnx";
    std::string face_recog_path= "D:\\QTwork\\face\\face_recognition_sface_2021dec.onnx";
    std::string face_data_path = "D:\\1";
    this->face_detector_recog.initFaceModels(face_detetor_path,face_recog_path,face_data_path);

}

void mainwindow::register_face()
{
    std::cout<<"register face action..."<<std::endl;
    bool ok;
    QString name= QInputDialog::getText(this,tr("人脸注册-输入名称"),tr("name:"),QLineEdit::Normal,QDir::home().dirName(),&ok);
    std::cout<<"register name: "<<name.toStdString()<<std::endl;
    if(this->myface.empty())
    {
        return;
    }
    this->face_detector_recog.registFace(this->myface,name.toStdString());
}

void mainwindow::load_image()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),"D:\\QTwork\\Saved Pictures",tr("Images (*.png *.xpm *.jpg);;videos(*.mp4 *.avi)"));
    if(fileName.isEmpty()){
        QMessageBox::warning(this,"warning!","Failed to open the image/video...");
        return;
    }
    qDebug()<<fileName<<Qt::endl;

    if(fileName.endsWith(".jpg")||fileName.endsWith(".png")){
        cv::Mat image = cv::imread(fileName.toStdString());
        if (image.empty()) {
            QMessageBox::warning(this,"warning!","Failed to load the image, it may be corrupted or in an unsupported format.");
            return;
        }
        image.copyTo(this->myface);
        std::vector<std::shared_ptr<faceInfo>> results;
        this->face_detector_recog.detectFace(image,results,this->showFPSOption->isChecked());
        if(this->matchFaceOption->isChecked()){
            this->face_detector_recog.matchFace(image,results,false);
        }
        for(const auto &oneface:results)
        {
            cv::Rect box;
            box.x=int(oneface->detResult.at<float>(0,0));
            box.y=int(oneface->detResult.at<float>(0,1));
            box.width=int(oneface->detResult.at<float>(0,2));
            box.height=int(oneface->detResult.at<float>(0,3));
            cv::rectangle(image,box,cv::Scalar(0,255,0),2,8,0);
            if(this->showlandmarkOption->isChecked()){
                cv::circle(image, cv::Point2i(int(oneface->detResult.at<float>(0,4)), int(oneface->detResult.at<float>(0,5))), 10, cv::Scalar(0, 255, 0), 2);
                cv::circle(image, cv::Point2i(int(oneface->detResult.at<float>(0,6)), int(oneface->detResult.at<float>(0,7))), 10, cv::Scalar(0, 255, 0), 2);
                cv::circle(image, cv::Point2i(int(oneface->detResult.at<float>(0,8)), int(oneface->detResult.at<float>(0,9))), 10, cv::Scalar(0, 255, 0), 2);
                cv::circle(image, cv::Point2i(int(oneface->detResult.at<float>(0,10)), int(oneface->detResult.at<float>(0,11))), 10, cv::Scalar(0, 255, 0), 2);
                cv::circle(image, cv::Point2i(int(oneface->detResult.at<float>(0,12)), int(oneface->detResult.at<float>(0,13))), 10, cv::Scalar(0, 255, 0), 2);
            }
            if(this->matchFaceOption->isChecked()){
                cv::putText(image,oneface->name,cv::Point(box.tl()),cv::FONT_HERSHEY_SIMPLEX,1.0,cv::Scalar(0,255,0));
            }
        }
        cv::Mat rgb;
        cv::cvtColor(image,rgb,cv::COLOR_BGR2RGB);
        this->canvas->setPixmap(QPixmap::fromImage(QImage(rgb.data,rgb.cols,rgb.rows,rgb.step,QImage::Format_RGB888)));
    }
    if(fileName.endsWith(".mp4")||fileName.endsWith(".avi")){
        std::cout<<"process video stream"<<std::endl;
        cv::VideoCapture capture(fileName.toStdString());
        cv::Mat frame;
        while(true)
        {
            bool ret=capture.read(frame);
            if(!ret){
                break;
            }
            std::vector<std::shared_ptr<faceInfo>> results;
            this->face_detector_recog.detectFace(frame,results,this->showFPSOption->isChecked());
            if(this->matchFaceOption->isChecked()){
                this->face_detector_recog.matchFace(frame,results,false);
            }

            for(const auto &oneface:results){
                cv::Rect box;
                box.x=int(oneface->detResult.at<float>(0,0));
                box.y=int(oneface->detResult.at<float>(0,1));
                box.width=int(oneface->detResult.at<float>(0,2));
                box.height=int(oneface->detResult.at<float>(0,3));
                cv::rectangle(frame,box,cv::Scalar(0,255,0),2,8,0);
                if(this->showlandmarkOption->isChecked()){
                    cv::circle(frame, cv::Point2i(int(oneface->detResult.at<float>(0,4)), int(oneface->detResult.at<float>(0,5))), 10, cv::Scalar(0, 255, 0), 2);
                    cv::circle(frame, cv::Point2i(int(oneface->detResult.at<float>(0,6)), int(oneface->detResult.at<float>(0,7))), 10, cv::Scalar(0, 255, 0), 2);
                    cv::circle(frame, cv::Point2i(int(oneface->detResult.at<float>(0,8)), int(oneface->detResult.at<float>(0,9))), 10, cv::Scalar(0, 255, 0), 2);
                    cv::circle(frame, cv::Point2i(int(oneface->detResult.at<float>(0,10)), int(oneface->detResult.at<float>(0,11))), 10, cv::Scalar(0, 255, 0), 2);
                    cv::circle(frame, cv::Point2i(int(oneface->detResult.at<float>(0,12)), int(oneface->detResult.at<float>(0,13))), 10, cv::Scalar(0, 255, 0), 2);
                }
                if(this->matchFaceOption->isChecked()){
                    cv::putText(frame,oneface->name,cv::Point(box.tl()),cv::FONT_HERSHEY_SIMPLEX,1.0,cv::Scalar(0,255,0));
                }
            }
            cv::Mat rgb;
            cv::cvtColor(frame,rgb,cv::COLOR_BGR2RGB);
            this->canvas->setPixmap(QPixmap::fromImage(QImage(rgb.data,rgb.cols,rgb.rows,rgb.step,QImage::Format_RGB888)));

            cv::waitKey(30);
        }
    }
}

void mainwindow::saveWindow()
{
    widget *newWidget = new widget();
    newWidget->setAttribute(Qt::WA_DeleteOnClose);
    newWidget->show();
}

mainwindow::~mainwindow()
{
    delete ui;
}
