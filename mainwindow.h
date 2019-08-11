#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "common_lib.hpp"
#include "utils.hpp"
#include <QMainWindow>
#include "capture.h"
#include "imageviewer.h"
#include "facedetection.h"
#include "facelandmark.h"
#include <memory>
#include <QThread>

class Thread final : public QThread { public: ~Thread() { quit(); wait(); } };

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void timerEvent(QTimerEvent * ev);
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_open_clicked();

    void on_btn_calibration_clicked();

private:
    void on_capture_started();

    void initWidgets();

signals:
    void reset_config();

private:
    Ui::MainWindow *ui;

    std::queue<FrameTask> tasks;
    std::unique_ptr<Capture> m_capture;
    std::unique_ptr<FaceDetection> m_detection;
    std::unique_ptr<FaceLandmark> m_landmark;
    std::unique_ptr<Thread> m_detectThread, m_landmarkThread;
    int capture_status;//0:close 1:open
    QBasicTimer m_timer;
    int first_frame;
};

void dlib_detect();


#endif // MAINWINDOW_H
