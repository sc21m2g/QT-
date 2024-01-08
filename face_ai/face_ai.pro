QT += core gui
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camerawindow.cpp \
    facealgo.cpp \
    main.cpp \
    mainwindow.cpp \
    widget.cpp

HEADERS += \
    camerawindow.h \
    facealgo.h \
    mainwindow.h \
    widget.h

FORMS += \
    mainwindow.ui \
    widget.ui

TRANSLATIONS += \
    face_ai_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# INCLUDEPATH +=D:\opencv\opencv_4.8.0\opencv\cudabuild\install\include`
# LIBS += -LD:\opencv\opencv_4.8.0\opencv\cudabuild\install\x64\vc17\lib
# LIBS += -lopencv_core480 -lopencv_highgui480 -lopencv_objdetect480 -lopencv_face480 -lopencv_imgproc480 -lopencv_videoio480 -lopencv_dnn480 -lopencv_imgcodecs480

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_core480
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_core480d

INCLUDEPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include
DEPENDPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_face480
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_face480d

INCLUDEPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include
DEPENDPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_highgui480
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_highgui480d

INCLUDEPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include
DEPENDPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_dnn480
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_dnn480d

INCLUDEPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include
DEPENDPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_imgcodecs480
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_imgcodecs480d

INCLUDEPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include
DEPENDPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_imgproc480
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_imgproc480d

INCLUDEPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include
DEPENDPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_objdetect480
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_objdetect480d

INCLUDEPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include
DEPENDPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_videoio480
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/x64/vc17/lib/ -lopencv_videoio480d

INCLUDEPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include
DEPENDPATH += $$PWD/../../opencv/opencv_4.8.0/opencv/cudabuild/install/include
