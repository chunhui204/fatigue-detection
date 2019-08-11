#include "facelandmark.h"

FaceLandmark::FaceLandmark(QObject *parent) : QObject(parent)
{
    std::string ert_name = "/home/liuch/C/test1/landmarks.dat";
    ert.reset(new ERT(ert_name));
    fe.reset(new FatigueEvent());

    m_timer.start(10, this);
    cur_time = get_cur_time();
}

void FaceLandmark::timerEvent(QTimerEvent * ev) {
    if (ev->timerId() != m_timer.timerId()) return;
    if(tasks.empty()) return;

    unsigned long cur = get_cur_time();
    FrameTask det_task = tasks.front();
    tasks.pop();

    std::map<std::string, double> metric;
    ert->landmarks_detect(det_task.frame, det_task.face_box, det_task.landmarks);

    int f = fe->process(det_task.landmarks, 1000./frame_rate, metric);

    if(false && f == -1) det_task.is_reseting = true;
    else
    {
        det_task.isFatigue = f > 0;
        det_task.metrics = metric;
        det_task.process = 2;
        det_task.is_reseting = false;
    }
    cur_time = cur;

    emit taskReady(det_task);
}

void FaceLandmark::on_reset_config()
{
    qDebug()<<"click";
    fe->on_reset_clicked();
}
