#include "facedetection.h"

FaceDetection::FaceDetection(QObject* parent) : QObject(parent),
    first(true),
    miss_val(0),
    iou_val(0)
{
    std::string pb_name = "/home/liuch/C/test1/mtcnn_frozen_model.pb";

    //create graph and session
    tensorflow::SessionOptions opts;
//    opts.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(0.5);
    opts.config.mutable_gpu_options()->set_allow_growth(true);
    TF_CHECK_OK(tensorflow::NewSession(opts, &session));
    load_graph(pb_name, graphdef, session, 0);
    m_timer.start(10, this);
}

FaceDetection::~FaceDetection()
{
    m_timer.stop();
}

void FaceDetection::timerEvent(QTimerEvent * ev) {
    if (ev->timerId() != m_timer.timerId()) return;
    if(tasks.empty()) return;
    cv::Mat image = tasks.front();
    tasks.pop();

    cv::Rect_<float> location;
    cv::Rect location_i;

    if(first || miss_val > 0.2 || iou_val <=0.5)
    {
        std::vector<face_box> face_info;
        mtcnn_detect(session, graphdef, image, face_info);
        if(!face_info.empty())
        {
            first=false;
            location_i.x = location.x = face_info[0].x0;
            location_i.y = location.y = face_info[0].y0;
            location_i.width = location.width = (face_info[0].x1-face_info[0].x0);
            location_i.height = location.height = (face_info[0].y1- face_info[0].y0);
            staple.tracker_staple_initialize(image, location);
            staple.tracker_staple_train(image, true);

            iou_val = 1;
            last = location;
            miss_val = miss_ratio(location, image.cols, image.rows);

            std::vector<cv::Point> lm;
            std::map<std::string,double> m;
            FrameTask task(image, 0, 1, location_i, lm, m, false);
            emit taskReady(task, frame_rate);
        }

    }
    else{
        location = staple.tracker_staple_update(image);
        location_i = location;
        staple.tracker_staple_train(image, false);

        iou_val = iou(last, location);
//            if(iou_val < 0.6)
        last = location;
        miss_val = miss_ratio(location, image.cols, image.rows);

        std::vector<cv::Point> lm;
        std::map<std::string,double> m;
        FrameTask task(image, 0, 1, location_i, lm, m, false);
        emit taskReady(task, frame_rate);
    }

}
