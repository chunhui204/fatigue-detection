#include "capture.h"
#include <QTimerEvent>
#include <QDebug>
Capture::Capture(QObject *parent) : QObject(parent), m_videoCapture(nullptr)
{
}

void Capture::start(int cam)
{
    if (nullptr == m_videoCapture)
    {
        m_videoCapture = new cv::VideoCapture("/home/liuch/Videos/sleep.mp4");
        m_frameRate = m_videoCapture->get(CV_CAP_PROP_FPS);
        qDebug() << "CV_CAP_PROP_FPS"<< m_frameRate;
    }
    if (m_videoCapture && m_videoCapture->isOpened()) {
        m_timer.start(20, this);// 50fps for vis
        emit started();
    }

}

void Capture::stop()
{
    m_timer.stop();
    m_videoCapture->release();
    delete m_videoCapture;
    m_videoCapture = nullptr;
}

void Capture::timerEvent(QTimerEvent * ev) {
        if (ev->timerId() != m_timer.timerId()) return;
        cv::Mat frame;
        if (!m_videoCapture->read(frame)) { // Blocks until a new frame is ready
            m_timer.stop();
            return;
        }
        emit matReady(frame, m_frameRate);
}

