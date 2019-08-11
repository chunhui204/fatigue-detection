#ifndef COMMON_LIB_H
#define COMMON_LIB_H

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>


struct face_landmark
{
    float x[5];
    float y[5];
};

struct face_box
{
    float x0; //anchor coordinations
    float y0;
    float x1;
    float y1;

    /* confidence score */
    float score;

    /*regression scale */

    float regress[4];//output regression, offset(division by image height or width) of anchor

    /* padding stuff*/
    float px0;
    float py0;
    float px1;
    float py1;

    face_landmark landmark;
};



class mtcnn {
    public:
        mtcnn(void){
            min_size_=40;
            pnet_threshold_=0.6;
            rnet_threshold_=0.7;
            onet_threshold_=0.9;
            factor_=0.709;

        }

        void set_threshold(float p, float r, float o)
        {
            pnet_threshold_=p;
            rnet_threshold_=r;
            onet_threshold_=o;
        }

        void set_factor_min_size(float factor, float min_size)
        {
            factor_=factor;
            min_size_=min_size;
        }


        virtual int load_model(const std::string& model_dir)=0;
        virtual void detect(cv::Mat& img, std::vector<face_box>& face_list)=0;
        virtual ~mtcnn(void){}

    protected:

        int min_size_;
        float pnet_threshold_;
        float rnet_threshold_;
        float onet_threshold_;
        float factor_;
};

/* factory part */

class mtcnn_factory
{
    public:

        typedef mtcnn * (*creator)(void);

        static void register_creator(const std::string& name, creator& create_func);
        static mtcnn * create_detector(const std::string& name);
                static std::vector<std::string> list(void);

    private:
        mtcnn_factory(){}


};

class  only_for_auto_register
{
    public:
        only_for_auto_register(std::string name, mtcnn_factory::creator func)
        {
            mtcnn_factory::register_creator(name,func);
        }

};

#define REGISTER_MTCNN_CREATOR(name,func) \
    static  only_for_auto_register dummy_mtcnn_creator_## name (#name, func)

//-----------------------------------------------------
#define NMS_UNION 1
#define NMS_MIN  2



struct scale_window
{
    int h;// current image height
    int w;// current image width
    float scale;// scale with origin image size
};
float iou(cv::Rect_<float>& input_a,  cv::Rect_<float>& input_b);

float miss_ratio(cv::Rect_<float>& input_a, float img_w, float img_h);

int numpy_round(float f);

void nms_boxes(std::vector<face_box>& input, float threshold, int type, std::vector<face_box>&output);

void regress_boxes(std::vector<face_box>& rects);

void square_boxes(std::vector<face_box>& rects);

void padding(int img_h, int img_w, std::vector<face_box>& rects);

void process_boxes(std::vector<face_box>& input, int img_h, int img_w, std::vector<face_box>& rects);

void generate_bounding_box(const float * confidence_data, int confidence_size,
               const float * reg_data, float scale, float threshold,
               int feature_h, int feature_w, std::vector<face_box>&  output, bool transposed);


void set_input_buffer(std::vector<cv::Mat>& input_channels,
        float* input_data, const int height, const int width);


void  cal_pyramid_list(int height, int width, int min_size, float factor,std::vector<scale_window>& list);

void cal_landmark(std::vector<face_box>& box_list);

#endif // COMMON_LIB_H
