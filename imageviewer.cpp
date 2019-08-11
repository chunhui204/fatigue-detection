#include "imageviewer.h"
#include <QDebug>

void ImageViewer::paintEvent(QPaintEvent *)
{
    if(m_img.isNull()) return;
    QPainter p(this);
//    int w = this->width(), h = this->height(), imgw = m_img.width(), imgh = m_img.height();
//    int x = (w-imgw)/2, y=(h-imgh)/2;

//    m_img.scaled(w, h, Qt::KeepAspectRatio);
    p.drawImage(0, 0, m_img);
    m_img = {};
}

void ImageViewer::setImage(const cv::Mat & frame)
{
    const QImage image((const uchar*)(frame.data), frame.cols, frame.rows,
                       QImage::Format_RGB888);
    image.bits();
    if (!m_img.isNull()) qDebug() << "Viewer dropped frame!";

//    if (m_img.size() != size()) setFixedSize(m_img.size());
    m_img = image;

    update();
}
