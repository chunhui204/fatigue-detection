#ifndef CAPTURE_H
#define CAPTURE_H

#include <QObject>
#include <QTimer>
#include <QImage>
#include <memory>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <QTimerEvent>

class Capture : public QObject
{
    Q_OBJECT
public:
    explicit Capture(QObject *parent = nullptr);


signals:
    void matReady(const cv::Mat, double frameRate);
    bool started();
public slots:
    void start(int);
    void stop();
private:
    void timerEvent(QTimerEvent * ev);

private:
    cv::VideoCapture* m_videoCapture;
    QBasicTimer m_timer;
    double m_frameRate;
};

#endif // CAPTURE_H
