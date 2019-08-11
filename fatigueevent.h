#ifndef FATIGUEEVENT_H
#define FATIGUEEVENT_H
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include "face_geometric.h"

#define TIME_PERIOD 5000 //ms
#define TIME_RESET 5000//
class FatigueEvent
{
public:
    FatigueEvent() : EAR_threshold(1.), ear_max(0.3), ear_min(10.), has_reset(true){}
    int process(const std::vector<cv::Point> &landmarks, double duration,std::map<std::string, double>& metric);
    void on_reset_clicked()
    {
        EARs.clear();
        eye_closing_time = 0;
        ear_min = 10.;
        time_tick = 0;
        has_reset = false;
    }



private:
     bool reset(const std::vector<cv::Point> &landmarks, float duration);
     float left_eye_aspect_ratio(const std::vector<cv::Point> &landmarks);
     float right_eye_aspect_ratio(const std::vector<cv::Point> &landmarks);
     double mouth_circle_ratio(const std::vector<cv::Point> &landmarks);
     float check_PERCLOS();
     void calc_dist_orient (const std::vector<cv::Point> &landmarks, float& dist, float& orient, bool left);
private:
    std::vector<float> EARs;
    float EAR_threshold;
    double eye_closing_time;
    float ear_max, ear_min;
    double time_tick;
    bool has_reset;
};

#endif // FATIGUEEVENT_H
