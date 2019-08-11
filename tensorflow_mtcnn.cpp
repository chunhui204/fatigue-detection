#include "tensorflow_mtcnn.hpp"

using std::string;
typedef unsigned char uint8;

void CVMat_to_Tensor(cv::Mat img, tensorflow::Tensor* output_tensor,int input_rows,int input_cols)
{
    //??????tensor??????
    float *p = output_tensor->flat<float>().data();

    //????Mat??tensor???????????Mat?????????tensor????
    cv::Mat tempMat(input_rows, input_cols, CV_32FC3, p);
    img.convertTo(tempMat, CV_32FC3);
}

bool load_graph(const string& pb_name, tensorflow::GraphDef &graphdef, tensorflow::Session *& session, int gpu_id)
{
//    tensorflow::GraphDef graphdef;
//    tensorflow::Session *session;
//    tensorflow::SessionOptions opts;
//    opts.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(0.5);
//    opts.config.mutable_gpu_options()->set_allow_growth(true);
//    TF_CHECK_OK(tensorflow::NewSession(opts, &session));
    tensorflow::Status status_load = tensorflow::ReadBinaryProto(tensorflow::Env::Default(), pb_name, &graphdef);
    tensorflow::graph::SetDefaultDevice("/cpu:"+ std::to_string(gpu_id), &graphdef);
    if(!status_load.ok())
    {
        std::cerr<<"pb file loading err.."<<std::endl;
        return -1;
    }
    tensorflow::Status status_create = session->Create(graphdef); //???????Session???
        if (!status_create.ok()) {
           std::cerr << "ERROR: Creating graph in session failed..." << status_create.ToString() << std::endl;
            return -1;
        }
    std::cout << "<----Successfully created session and load graph.------->"<< std::endl;

    return 0;
}

void generate_bounding_box_tf(tensorflow::TTypes<float, 4>::Tensor &confidence_data,
                              tensorflow::TTypes<float, 4>::Tensor& reg_data,
                              float scale, float threshold, int feature_h, int feature_w,
                              std::vector<face_box>&  output, bool transposed)
{

    int stride = 2;
    int cellSize = 12;

    int img_h= feature_h;
    int img_w = feature_w;


    for(int y=0;y<img_h;y++)
        for(int x=0;x<img_w;x++)
        {
            float score = confidence_data(0, y, x, 1);
            if(score>= threshold)
            {
                float top_x = (int)((x*stride + 1) / scale);
                float top_y = (int)((y*stride + 1) / scale);
                float bottom_x = (int)((x*stride + cellSize) / scale);
                float bottom_y = (int)((y*stride + cellSize) / scale);

                face_box box;

                box.x0 = top_x;
                box.y0 = top_y;
                box.x1 = bottom_x;
                box.y1 = bottom_y;

                box.score=score;
                if(transposed)
                {

                    box.regress[1]=reg_data(0,y,x,0);
                    box.regress[0]=reg_data(0,y,x,1);
                    box.regress[3]=reg_data(0,y,x,2);
                    box.regress[2]= reg_data(0,y,x,3);
                }
                else {

                    box.regress[0]=reg_data(0,y,x,0);
                    box.regress[1]=reg_data(0,y,x,1);
                    box.regress[2]=reg_data(0,y,x,2);
                    box.regress[3]= reg_data(0,y,x,3);
                }

                output.push_back(box);
            }

        }
}

/* To make tensor release happy...*/
static void dummy_deallocator(void* data, size_t len, void* arg)
{
}

void run_PNet(tensorflow::Session * &session, tensorflow::GraphDef &graph, cv::Mat& img, scale_window& win, std::vector<face_box>& box_list)
{
    cv::Mat  resized;
    int scale_h=win.h;
    int scale_w=win.w;
    float scale=win.scale;
    float pnet_threshold=0.6;
//convert to rgb first
    cv::resize(img, resized, cv::Size(scale_w, scale_h),0,0);

    tensorflow::Tensor input_tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({1,scale_h,scale_w,3}));
    CVMat_to_Tensor(resized, &input_tensor, scale_h, scale_w);
    /* tensorflow related*/
    std::vector<tensorflow::Tensor> outputs;
    TF_CHECK_OK(session->Run({{"pnet/input:0", input_tensor}}, {"pnet/conv4-2/BiasAdd:0", "pnet/prob1:0"}, {}, &outputs));

    /*retrieval the forward results*/
    auto conf_tensor = outputs[1].tensor<float, 4>();
    auto reg_tensor = outputs[0].tensor<float, 4>();

    int feature_h= conf_tensor.dimensions()[1];
    int feature_w= conf_tensor.dimensions()[2];

    std::vector<face_box> candidate_boxes;

    generate_bounding_box_tf(conf_tensor, reg_tensor,
            scale, pnet_threshold, feature_h, feature_w, candidate_boxes, true);

    nms_boxes(candidate_boxes, 0.5, NMS_UNION, box_list);
}


void copy_one_patch(const cv::Mat& img, face_box&input_box, float* data_to, int height, int width)
{

    cv::Mat resized(height,width,CV_32FC3, data_to);

    //crop patch(with bbox) from img
    cv::Mat chop_img = img(cv::Range(input_box.py0,input_box.py1),
            cv::Range(input_box.px0, input_box.px1));

    int pad_top = std::abs(input_box.py0 - input_box.y0);
    int pad_left = std::abs(input_box.px0 - input_box.x0);
    int pad_bottom = std::abs(input_box.py1 - input_box.y1);
    int pad_right = std::abs(input_box.px1-input_box.x1);

    cv::copyMakeBorder(chop_img, chop_img, pad_top, pad_bottom,pad_left, pad_right,  cv::BORDER_CONSTANT, cv::Scalar(0));

    cv::resize(chop_img,resized, cv::Size(width, height), 0, 0);
}


void run_RNet(tensorflow::Session * &session, tensorflow::GraphDef &graph, cv::Mat& img, std::vector<face_box>& pnet_boxes, std::vector<face_box>& output_boxes)
{
    int batch=pnet_boxes.size();
    int channel = 3;
    int height = 24;
    int width = 24;

    float rnet_threshold=0.7;

    /* prepare input image data */
    tensorflow::Tensor input_tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({batch ,height, width, channel}));
    float *input_data = input_tensor.flat<float>().data();

    for(int i=0;i<batch;i++)
    {
        int patch_size=width*height*channel;

        copy_one_patch(img,pnet_boxes[i], input_data, height, width);

        input_data+=patch_size;
    }

    /* tensorflow  related */
    std::vector<tensorflow::Tensor> outputs;
    TF_CHECK_OK(session->Run({{"rnet/input:0", input_tensor}}, {"rnet/conv5-2/conv5-2:0", "rnet/prob1:0"}, {}, &outputs));

    /*retrieval the forward results*/
    auto conf_tensor = outputs[1].tensor<float, 2>();
    auto reg_tensor = outputs[0].tensor<float, 2>();

    for(int i=0;i<batch;i++)
    {

        if(conf_tensor(i, 1) > rnet_threshold)
        {
            face_box output_box;

            face_box& input_box=pnet_boxes[i];

            output_box.x0=input_box.x0;
            output_box.y0=input_box.y0;
            output_box.x1=input_box.x1;
            output_box.y1=input_box.y1;

            output_box.score = conf_tensor(i, 1);

            /*Note: regress's value is swaped here!!!*/

            output_box.regress[0]= reg_tensor(i, 1);
            output_box.regress[1]=reg_tensor(i, 0);
            output_box.regress[2]=reg_tensor(i, 3);
            output_box.regress[3]=reg_tensor(i, 2);

            output_boxes.push_back(output_box);

        }
    }
}

void run_ONet(tensorflow::Session * &session, tensorflow::GraphDef &graph, cv::Mat& img, std::vector<face_box>& rnet_boxes, std::vector<face_box>& output_boxes)
{
    int batch=rnet_boxes.size();
    int channel = 3;
    int height = 48;
    int width = 48;

    float onet_threshold=0.9;

    /* prepare input image data */

    int  input_size=batch*height*width*channel;

    tensorflow::Tensor input_tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({batch ,height, width, channel}));
    float *input_data = input_tensor.flat<float>().data();

    for(int i=0;i<batch;i++)
    {
        int patch_size=width*height*channel;

        copy_one_patch(img, rnet_boxes[i], input_data, height,width);

        input_data+=patch_size;
    }


    /* tensorflow  related */

    std::vector<tensorflow::Tensor> outputs;
    TF_CHECK_OK(session->Run({{"onet/input:0", input_tensor}}, {"onet/conv6-2/conv6-2:0", "onet/conv6-3/conv6-3:0", "onet/prob1:0"}, {}, &outputs));

    /*retrieval the forward results*/
    auto reg_tensor = outputs[0].tensor<float, 2>();
    auto points_tensor = outputs[1].tensor<float, 2>();
    auto conf_tensor = outputs[2].tensor<float, 2>();
    /*retrieval the forward results*/
    for(int i=0;i<batch;i++)
    {
        float sc = conf_tensor(i, 1);
        if(sc > onet_threshold)
        {
            face_box output_box;

            face_box& input_box=rnet_boxes[i];

            output_box.x0=input_box.x0;
            output_box.y0=input_box.y0;
            output_box.x1=input_box.x1;
            output_box.y1=input_box.y1;

            output_box.score = sc;

            output_box.regress[0]= reg_tensor(i, 1);
            output_box.regress[1]=reg_tensor(i, 0);
            output_box.regress[2]=reg_tensor(i, 3);
            output_box.regress[3]=reg_tensor(i, 2);

            /*Note: switched x,y points value too..*/
            for (int j = 0; j<5; j++){
                output_box.landmark.x[j] =points_tensor(i, j+5);
                output_box.landmark.y[j] = points_tensor(i, j);
            }

            output_boxes.push_back(output_box);


        }
    }
}


void mtcnn_detect(tensorflow::Session * &sess, tensorflow::GraphDef& graph, cv::Mat& img,
                  std::vector<face_box>& face_list)
{
    cv::Mat working_img;

    float alpha=0.0078125;
    float mean=127.5;

    img.convertTo(working_img, CV_32FC3);

    working_img=(working_img-mean)*alpha;

    working_img=working_img.t();

    cv::cvtColor(working_img,working_img, cv::COLOR_BGR2RGB);

    int img_h=working_img.rows;
    int img_w=working_img.cols;

    int min_face=20;
    float factor=0.709;


    std::vector<scale_window> win_list;

    std::vector<face_box> total_pnet_boxes;
    std::vector<face_box> total_rnet_boxes;
    std::vector<face_box> total_onet_boxes;


    cal_pyramid_list(img_h,img_w, min_face, factor, win_list);

    for(unsigned int i=0;i<win_list.size();i++)
    {
        std::vector<face_box>boxes;

        run_PNet(sess, graph, working_img, win_list[i], boxes);

        total_pnet_boxes.insert(total_pnet_boxes.end(),boxes.begin(),boxes.end());
    }


    std::vector<face_box> pnet_boxes;
    process_boxes(total_pnet_boxes,img_h,img_w,pnet_boxes);


    // RNet
    std::vector<face_box> rnet_boxes;

    run_RNet(sess, graph,working_img, pnet_boxes,total_rnet_boxes);

    process_boxes(total_rnet_boxes,img_h,img_w,rnet_boxes);


    //ONet
    run_ONet(sess,graph,working_img, rnet_boxes,total_onet_boxes);

    //calculate the landmark

    for(unsigned int i=0;i<total_onet_boxes.size();i++)
    {
        face_box& box=total_onet_boxes[i];

        float h=box.x1-box.x0+1;
        float w=box.y1-box.y0+1;

        for(int j=0;j<5;j++)
        {
            box.landmark.x[j]=box.x0+w*box.landmark.x[j]-1;
            box.landmark.y[j]=box.y0+h*box.landmark.y[j]-1;
        }

    }


    //Get Final Result

    regress_boxes(total_onet_boxes);
    nms_boxes(total_onet_boxes, 0.7, NMS_MIN, face_list);

    //switch x and y, since working_img is transposed

    for(unsigned int i=0;i<face_list.size();i++)
    {
        face_box& box=face_list[i];

        std::swap(box.x0,box.y0);
        std::swap(box.x1,box.y1);

        for(int l=0;l<5;l++)
        {
            std::swap(box.landmark.x[l],box.landmark.y[l]);
        }
    }

//    face_box_adjustment(face_list_t, face_list, (float)img_h, (float)img_w);

}
