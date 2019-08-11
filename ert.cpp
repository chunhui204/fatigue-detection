#include "ert.h"

using namespace dlib;
ERT::ERT(std::string &model_path)
{
    deserialize(model_path) >> detector;
}

void ERT::landmarks_detect(const cv::Mat& mat, const cv::Rect &bbox, std::vector<cv::Point> &landmarks)
{
    array2d<rgb_pixel> img;
    cv_image<rgb_pixel> cvimg = mat;
    assign_image(img, cvimg);

    rectangle det(bbox.x, bbox.y, bbox.x + bbox.width, bbox.y + bbox.height);
    full_object_detection d = detector(img, det);

    landmarks.clear();
    for(int l=0;l<68;l++)
    {
        landmarks.push_back(cv::Point(d.part(l).x(), d.part(l).y()));
    }
}
