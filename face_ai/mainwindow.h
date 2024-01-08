#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "facealgo.h"
#include "qcheckbox.h"
#include "qmessagebox.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class mainwindow;
}
QT_END_NAMESPACE

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();
    void register_face();
    void load_image();
    void saveWindow();
    FaceAlgo face_detector_recog;

private:
    Ui::mainwindow *ui;
    cv::Mat myface;
    QCheckBox *matchFaceOption;
    QCheckBox *showlandmarkOption;
    QCheckBox *showFPSOption;
    QLabel *canvas;
    cv::VideoCapture camera;
    QTimer *timer;
};
#endif // MAINWINDOW_H
