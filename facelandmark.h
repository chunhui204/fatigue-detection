#ifndef FACELANDMARK_H
#define FACELANDMARK_H

#include <QObject>
#include <dlib/opencv.h>
#include <dlib/image_io.h>
#include <iostream>
#include "utils.hpp"
#include "common_lib.hpp"
#include "fatigueevent.h"
#include <QTimerEvent>
#include <QTimer>
#include <memory>
#include "ert.h"

class FaceLandmark : public QObject
{
    Q_OBJECT

    void timerEvent(QTimerEvent * ev);

public:
    explicit FaceLandmark(QObject *parent = nullptr);
    ~FaceLandmark(){m_timer.stop();}
    void queue(FrameTask task, double rate){
        tasks.push(task);
        frame_rate = rate;
    }
    void on_reset_config();
signals:
    void taskReady(FrameTask task);

private:
    std::queue<FrameTask> tasks;
    QBasicTimer m_timer;
    std::unique_ptr<ERT> ert;
    std::unique_ptr<FatigueEvent> fe;
    unsigned long cur_time;
    double frame_rate;
};

#endif // FACELANDMARK_H
