#ifndef FACE_GEOMETRIC_H
#define FACE_GEOMETRIC_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>



void dist_point_to_line(const std::vector<cv::Point>& line_points, const cv::Point& p, float& dist, float& orient);
#endif // FACE_GEOMETRIC_H
