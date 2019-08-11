#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>

class ImageViewer : public QWidget {
    Q_OBJECT
    QImage m_img;
    void paintEvent(QPaintEvent *) ;

public:
    ImageViewer(QWidget * parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_OpaquePaintEvent);
    }

    void setImage(const cv::Mat & img);
};
#endif // IMAGEVIEWER_H
