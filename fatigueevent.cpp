#include "fatigueevent.h"
#include <QDebug>
#include <QDateTime>

float FatigueEvent::left_eye_aspect_ratio(const std::vector<cv::Point> &landmarks)
{
    int start = 36;
    long dist1 = std::sqrt(std::pow(landmarks[start+1].x - landmarks[start+5].x, 2) +
                           pow(landmarks[start+1].y - landmarks[start+5].y, 2));

    long dist2 = std::sqrt(std::pow(landmarks[start+2].x - landmarks[start+4].x, 2) +
                           pow(landmarks[start+2].y - landmarks[start+4].y, 2));

    long dist3 = std::sqrt(std::pow(landmarks[start].x - landmarks[start+3].x, 2) +
                           pow(landmarks[start].y - landmarks[start+3].y, 2));
    return float(dist1 + dist2) / 2 / dist3;
}

float FatigueEvent::right_eye_aspect_ratio(const std::vector<cv::Point> &landmarks)
{
    int start = 42;
    long dist1 = std::sqrt(std::pow(landmarks[start+1].x - landmarks[start+5].x, 2) +
                           pow(landmarks[start+1].y - landmarks[start+5].y, 2));

    long dist2 = std::sqrt(std::pow(landmarks[start+2].x - landmarks[start+4].x, 2) +
                           pow(landmarks[start+2].y - landmarks[start+4].y, 2));

    long dist3 = std::sqrt(std::pow(landmarks[start].x - landmarks[start+3].x, 2) +
                           pow(landmarks[start].y - landmarks[start+3].y, 2));
    return float(dist1 + dist2) / 2 / dist3;
}

double FatigueEvent::mouth_circle_ratio(const std::vector<cv::Point> &landmarks)
{
    std::vector<cv::Point> points;
    for(int i=60;i<68;i++) points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
    double A =  cv::contourArea(points);
    double P = cv::arcLength(points, true);

    return 4*3.1415*A / P / P;
}

void FatigueEvent::calc_dist_orient (const std::vector<cv::Point> &landmarks, float& dist, float& orient, bool left){
    float x = landmarks[27].x, y = landmarks[27].y;
    for(int i=28;i<=30;i++)
    {
        x+= landmarks[i].x;
        y+= landmarks[i].y;
    }
    x = x / 4;
    y = y/4;
    cv::Point p(x,y);

    std::vector<cv::Point> points;
    int start = left? 0:12;
    for(int i=start;i<start+5;i++)
    {
        points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
    }
    dist_point_to_line(points, p, dist, orient);
}
/**
 * @brief FatigueEvent::process
 * @param landmarks
 * @param duration
 * @param metric
 * @return 1: fatigue 0:normal -1:reseting
 */
int FatigueEvent::process(const std::vector<cv::Point> &landmarks, double duration, std::map<std::string, double>& metric)
{
    static int mouth_cnt = 0;
    static int eye_cnt = 0;
    //reset
    if(has_reset == false)
    {
        if(reset(landmarks, duration)) has_reset = true;
        return -1;
    }

    float EAR1 = left_eye_aspect_ratio(landmarks);
    float EAR2 = right_eye_aspect_ratio(landmarks);
    float EAR = (EAR1 + EAR2)/ 2;

    double e =mouth_circle_ratio(landmarks);
    if(e >= 0.5) mouth_cnt++;
    if(EAR <= EAR_threshold) eye_cnt++;

    float d1, orient1, d2, orient2;
    calc_dist_orient(landmarks, d1, orient1, true);
    calc_dist_orient(landmarks, d2, orient2, false);

    //if()
    metric["EAR1"] = EAR1;
    metric["EAR2"] = EAR2;
    metric["EAR"] = EAR;
    metric["mouth"] = e;
    metric["dur"] = time_tick;
    metric["close_time"] = eye_closing_time;
    metric["eye"] = EAR<EAR_threshold?1:0;
    metric["d1"] = d1;
    metric["d2"] = d2;
    metric["attention"] = 1;
    if(d1 < d2/3 || d2<d1/3) metric["attention"] = 0;


    if(EAR <= EAR_threshold && (EARs.empty() || EARs.back() <= EAR_threshold))
        eye_closing_time += duration;
    else
        eye_closing_time = 0;

    EARs.push_back(EAR);
    time_tick += duration;

    //check period
    int flag = 0;
    metric["1"] = -1;
    metric["2"] = -1;
    metric["3"] =-1;
    metric["4"] = -1;
    metric["5"] = -1;
    if(time_tick >= TIME_PERIOD)
    {
        unsigned int n = EARs.size();
        //eye closing
        float PERCLOS = check_PERCLOS();
        if(PERCLOS >= 0.3)
        {
            flag = 1;
//            qDebug() << "PERCOLS";
        }

        //mouth closing
        if(float(mouth_cnt) / n >= 0.15)
        {
            flag = 1;
//            qDebug() << "mouth";
        }

        if( float(eye_cnt)/n >= 0.3)
        {
            flag = 1;
//            qDebug()<<"eye";
        }
       metric["1"] = float(eye_cnt)/n;
       metric["2"] = float(mouth_cnt)/n;
       metric["3"] =PERCLOS;
       metric["4"] = EAR_threshold;
       metric["5"] = duration;
        //variable reset
        EARs.clear();
        eye_cnt = 0;
        mouth_cnt=0;
        time_tick = 0;
    }
    if(eye_closing_time >= 2000)
    {
        flag = 1;
        eye_closing_time = 0;
    }
    metric["f"] = flag;

    return flag;
}

float FatigueEvent::check_PERCLOS()
{
    int cnt=0, n = EARs.size();
    for(int i=0;i< n; i++)
    {
        if(EARs[i] < EAR_threshold) cnt++;
    }
    std::sort(EARs.begin(), EARs.end());
    //update EAR threshold, below thres bong to eye closing.
    ear_min = std::max(std::min(EARs[int(0.01*n)], ear_min), 0.1f);

    EAR_threshold = ear_min + 0.4* (ear_max - ear_min);

//    qDebug() << "max_ear "<< ear_max <<" min_ear " << ear_min <<"thres"<< EAR_threshold;
//    qDebug() << "PERCLOS: "<< float(cnt)/ n;
    return float(cnt)/  n;

}
 bool FatigueEvent::reset(const std::vector<cv::Point> &landmarks, float duration)
 {
     float EAR1 = left_eye_aspect_ratio(landmarks);
     float EAR2 = right_eye_aspect_ratio(landmarks);
     EARs.push_back(EAR1);
     EARs.push_back(EAR2);
     time_tick += duration;

     if(time_tick >= TIME_RESET)
     {
         unsigned int n = EARs.size();
         std::sort(EARs.begin(), EARs.end());
         ear_max = std::max(EARs[int(0.99*n)], ear_max);
         ear_min = std::min(EARs[int(0.01*n)], ear_min);
         EAR_threshold = 0.1 + 0.4* (ear_max - 0.1);

         EARs.clear();
         eye_closing_time = 0;
         ear_min = 10.;
         time_tick = 0;
         return true;
     }
     return false;
 }

