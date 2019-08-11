#-------------------------------------------------
#
# Project created by QtCreator 2019-07-28T07:23:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /home/liuch/opencv/include \
/home/liuch/opencv/include/opencv \
/home/liuch/opencv/include/opencv2 \
/home/liuch/tensorflow/google/tensorflow \
/home/liuch/eigen3/include/eigen3 \
#/home/liuch/OpenBLAS \
/home/liuch/dlib-19.17

LIBS += /home/liuch/tensorflow/lib/libtensorflow_cc.so \
/home/liuch/tensorflow/lib/libtensorflow_framework.so \
/home/liuch/opencv/lib/libopencv_calib3d.so \
/home/liuch/opencv/lib/libopencv_shape.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_calib3d.so.3.4 \
/home/liuch/opencv/lib/libopencv_stitching.so \
/home/liuch/opencv/lib/libopencv_calib3d.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_stitching.so.3.4 \
/home/liuch/opencv/lib/libopencv_core.so \
/home/liuch/opencv/lib/libopencv_stitching.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_core.so.3.4 \
/home/liuch/opencv/lib/libopencv_superres.so \
/home/liuch/opencv/lib/libopencv_core.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_superres.so.3.4 \
/home/liuch/opencv/lib/libopencv_dnn.so \
/home/liuch/opencv/lib/libopencv_superres.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_dnn.so.3.4 \
/home/liuch/opencv/lib/libopencv_videoio.so \
/home/liuch/opencv/lib/libopencv_dnn.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_videoio.so.3.4 \
/home/liuch/opencv/lib/libopencv_features2d.so \
/home/liuch/opencv/lib/libopencv_videoio.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_features2d.so.3.4 \
/home/liuch/opencv/lib/libopencv_video.so \
/home/liuch/opencv/lib/libopencv_features2d.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_video.so.3.4 \
/home/liuch/opencv/lib/libopencv_flann.so \
/home/liuch/opencv/lib/libopencv_video.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_flann.so.3.4 \
/home/liuch/opencv/lib/libopencv_videostab.so \
/home/liuch/opencv/lib/libopencv_flann.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_videostab.so.3.4 \
/home/liuch/opencv/lib/libopencv_highgui.so \
/home/liuch/opencv/lib/libopencv_videostab.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_highgui.so.3.4 \
/home/liuch/opencv/lib/libopencv_highgui.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_imgcodecs.so \
/home/liuch/opencv/lib/libopencv_imgcodecs.so.3.4 \
/home/liuch/opencv/lib/libopencv_imgcodecs.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_imgproc.so \
/home/liuch/opencv/lib/libopencv_imgproc.so.3.4 \
/home/liuch/opencv/lib/libopencv_imgproc.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_ml.so \
/home/liuch/opencv/lib/libopencv_ml.so.3.4 \
/home/liuch/opencv/lib/libopencv_ml.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_objdetect.so \
/home/liuch/opencv/lib/libopencv_objdetect.so.3.4 \
/home/liuch/opencv/lib/libopencv_objdetect.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_photo.so \
/home/liuch/opencv/lib/libopencv_photo.so.3.4 \
/home/liuch/opencv/lib/libopencv_photo.so.3.4.4 \
/home/liuch/opencv/lib/libopencv_shape.so \
/home/liuch/opencv/lib/libopencv_shape.so.3.4 \
-L/home/liuch/dlib-19.17/dlib
#/home/liuch/dlib-19.17/build/dlib/libdlib.a

CONFIG += c++11
CONFIG += static

QMAKE_CXXFLAGS += -std=c++0x -DDLIB_PNG_SUPPORT -DDLIB_JPEG_SUPPORT

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    test_dlib.cpp \
    utils.cpp \
    common_lib.cpp \
    tensorflow_mtcnn.cpp \
    face_box_utils.cpp \
    staple_tracker.cpp \
    fhog.cpp \
    fatigueevent.cpp \
    eye_center.cpp \
    face_geometric.cpp \
    capture.cpp \
    imageviewer.cpp \
    facedetection.cpp \
    facelandmark.cpp \
    ert.cpp

HEADERS += \
        mainwindow.h \
    common_lib.hpp \
    utils.hpp \
    tensorflow_mtcnn.hpp \
    face_box_utils.hpp \
    staple_tracker.hpp \
    sse.hpp \
    fhog.h \
    fatigueevent.h \
    eye_center.h \
    face_geometric.h \
    capture.h \
    imageviewer.h \
    facedetection.h \
    facelandmark.h \
    ert.h

FORMS += \
        mainwindow.ui

SOURCES += /home/liuch/dlib-19.17/dlib/all/source.cpp
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

