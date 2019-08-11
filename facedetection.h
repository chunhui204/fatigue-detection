#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include <QObject>
#include <queue>
#include <opencv2/opencv.hpp>
#include "cv.h"
#include "highgui.h"
#include <tensorflow/core/platform/env.h>
#include <tensorflow/core/public/session.h>
#include <utils.hpp>
#include <QTimerEvent>
#include <QTimer>
#include <memory>
#include "common_lib.hpp"
#include "tensorflow_mtcnn.hpp"
#include "staple_tracker.hpp"
class FaceDetection: public QObject
{
    Q_OBJECT

    void timerEvent(QTimerEvent * ev);

public:
    explicit FaceDetection(QObject *parent = nullptr);
    ~FaceDetection();
    void queue(cv::Mat frame, double rate)
    {
        tasks.push(frame);
        frame_rate = rate;
    }
    void initialize()
    {
        std::queue<cv::Mat> empty;
        swap(empty, tasks);

        iou_val=1.;
        miss_val = 0;
        first=true;
    }

signals:
    void taskReady(FrameTask task, double rate);
private:
    tensorflow::GraphDef graphdef;
    tensorflow::Session *session;

    std::queue<cv::Mat> tasks;
    QBasicTimer m_timer;
    bool first;
    STAPLE_TRACKER staple;
    float iou_val;
    cv::Rect_<float> last;
    float miss_val;
    double frame_rate;
};


#endif // FACEDETECTION_H
