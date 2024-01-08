#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QWidget>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>


class CameraWindow : public QWidget {
    Q_OBJECT
public:
    CameraWindow(const QString& someString,QWidget *parent = nullptr);
    ~CameraWindow();
private:
    QCamera *camera;
    QMediaCaptureSession *captureSession;
    QVideoSink *videoSink;
};

#endif // CAMERAWINDOW_H
