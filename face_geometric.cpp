#include "face_geometric.h"


int check_point_orient(float k, float x1, float y1, const cv::Point& p)
{
    float x = (y1 - p.y) / k + x1;
    if(x < p.x) return -1;//point is right of line
    if(x > p.x) return 1;//point is left of line
    else return 0;
}


void dist_point_to_line(const std::vector<cv::Point>& line_points, const cv::Point& p, float& dist, float& orient){
    cv::Vec4f line;
    cv::fitLine(line_points,
                line,
                CV_DIST_HUBER,
                0,
                0.01,
                0.01);
    float k = line[1]/line[0],  b = line[3]-k*line[2], x1 = line[2], y1 = line[3];
//    cv::line(image, cv::Point(0.f,b), cv::Point((100.f-b)/k, 100.f), cv::Scalar(255, 255, 0),1);
    dist = std::abs(k*(p.x - x1) + p.y-y1);
    dist = dist / std::sqrt(k*k + 1);
    orient = check_point_orient(k, x1, y1, p);
}

