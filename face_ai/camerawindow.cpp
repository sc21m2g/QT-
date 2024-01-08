#include "camerawindow.h"
#include "qlabel.h"
#include <QVBoxLayout>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QtMultimedia>

CameraWindow::CameraWindow(const QString& someString,QWidget *parent) : QWidget(parent) {
    // 查找后置相机
    QCameraDevice defaultCameraDevice;

    // Retrieve the list of available camera devices
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();

    // Iterate through the available devices to find a back-facing camera
    for (const QCameraDevice &cameraDevice : cameras) {
        if (cameraDevice.position() == QCameraDevice::Position::BackFace) {
            defaultCameraDevice = cameraDevice;
            break;
        }
    }

    // 使用找到的后置相机创建一个相机对象
    camera = new QCamera(defaultCameraDevice);

    // 创建 QMediaCaptureSession 实例
    captureSession = new QMediaCaptureSession;

    // 创建 QVideoSink 实例
    videoSink = new QVideoSink;

    // 设置 capture session 的视频输出
    captureSession->setCamera(camera);
    captureSession->setVideoSink(videoSink);

    // 创建一个视频处理的 QLabel 作为占位符显示
    auto videoOutput = new QLabel(this);
    videoOutput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    videoOutput->setAlignment(Qt::AlignCenter);

    // 把 QLabel 添加到窗口布局
    auto layout = new QVBoxLayout(this);
    layout->addWidget(videoOutput);

    // 开始捕获
    // 绑定 videoSink 的 frameReady 信号
    connect(videoSink, &QVideoSink::videoFrameChanged, this, [videoOutput](const QVideoFrame &frame) {
        // 在此处处理视频帧，例如转换为 QImage 然后设置到 videoOutput
        if (!frame.isValid())
            return;

        QImage image = frame.toImage();
        if (!image.isNull()) {
            videoOutput->setPixmap(QPixmap::fromImage(image));
        }
    });

    // 启动相机
    camera->start();
}

CameraWindow::~CameraWindow() {
    // 释放资源
    camera->stop();
    delete videoSink;
    delete captureSession;
    delete camera;
}
