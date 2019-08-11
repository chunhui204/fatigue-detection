#ifndef ERT_H
#define ERT_H

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>


class ERT
{
public:
    ERT(std::string& model_path);
    void landmarks_detect(const cv::Mat& img, const cv::Rect &bbox, std::vector<cv::Point> &landmarks);

private:
    dlib::shape_predictor detector;
};
#endif // ERT_H
