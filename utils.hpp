#ifndef UTILS_H
#define UTILS_H

#include <QDebug>
#include <sys/time.h>
#include <map>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

struct FrameTask
{
public:
    FrameTask(){}
    FrameTask(const cv::Mat &f, int t, int p, const cv::Rect& box,
              const std::vector<cv::Point>& lm, const std::map<std::string, double> &mt,
              bool i) : frame(f), track_id(t),process(p), face_box(box), landmarks(lm),
                        metrics(mt), isFatigue(i), is_reseting(false){}
    void release()
    {
        frame.release();
        landmarks.clear();
        metrics.clear();
    }

    cv::Mat frame;
    int track_id;
    int process;
    cv::Rect face_box;
    std::vector<cv::Point> landmarks;
    std::map<std::string, double> metrics;
    bool isFatigue;
    bool attention;
    bool is_reseting;

};

unsigned long get_cur_time(void);
#endif // UTILS_H
