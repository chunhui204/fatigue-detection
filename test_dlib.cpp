#include "mainwindow.h"

using namespace dlib;
using namespace std;
//using namespace cv; 和dlib的命名空间有冲突

//void dlib_detect()
//{
//    string face_cascade_name = "/home/liuch/opencv/share/OpenCV/haarcascades/haarcascade_frontalface_alt2.xml";
//    //这里使用的LBP检测器，速度较haar检测器速度快，没有的话使用opencv自带的haar特征检测器也可以
//    cv::CascadeClassifier face_cascade;
//    face_cascade.load(face_cascade_name);

//    shape_predictor sp_detector;
//    string shape_model = "/home/liuch/C/test1/shape_predictor_68_face_landmarks.dat";
//    //模型下载地址如下:
//    //http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2
//    deserialize(shape_model) >> sp_detector;

//    string img_path = "/home/liuch/cy.jpg";
//    cv::VideoCapture cap("/home/liuch/Videos/wt-glass.mp4");
//    double rate = cap.get(CV_CAP_PROP_FPS);
//    while(true)
//    {

////        load_image(img, img_path);
//        cv::Mat face ;//= cv::imread(img_path);

//        cap>>face;
//        if(face.empty()) break;
//        array2d<rgb_pixel> img;
//        cv_image<rgb_pixel> cvimg = face;
//        assign_image(img, cvimg);
//        std::vector<cv::Rect> faces;
//        cv::Mat face_gray;
//        cv::cvtColor( face, face_gray, CV_BGR2GRAY );  //rgb类型转换为灰度类型
//        cv::equalizeHist( face_gray, face_gray );   //直方图均衡化

//        clock_t start_t = clock();
//        face_cascade.detectMultiScale(face_gray,faces,1.2,2,0|CV_HAAR_FIND_BIGGEST_OBJECT,cv::Size(20,20));

//    // shape predict
//        std::vector<full_object_detection> shapes;

//       for(int i=0;i<faces.size();i++)
//       {
//           dlib::rectangle det;
//           //将opencv检测到的矩形转换为dlib需要的数据结构，这里没有判断检测不到人脸的情况
//           det.set_left(faces[i].x);
//           det.set_top(faces[i].y);
//           det.set_right(faces[i].x+faces[i].width);
//           det.set_bottom(faces[i].y+faces[i].height);
//           full_object_detection shape = sp_detector(img, det);
//           cout<<"face id "<<to_string(i)<<endl;
//           cout << "number of parts: "<< shape.num_parts() << endl;
//           cout << "pixel position of first part:  " << shape.part(0) << endl;
//           cout << "pixel position of second part: " << shape.part(1) << endl;

//           shapes.push_back(shape);
//       }
//       clock_t end_t = clock();
//       cout<<"elasp time "<< double(end_t-start_t)/1000<<endl;
//        //xianshi
//       for(int i=0;i<faces.size();i++)
//       {
//           cv::rectangle(face, faces[i], cv::Scalar(0, 255,0), 2);
//       }
//       for(int j=0;j<shapes.size();j++)
//       {
//           for (int i = 0; i < 68; i++) {
//               cv::circle(face, cvPoint(shapes[j].part(i).x(), shapes[j].part(i).y()), 3, cv::Scalar(0, 0, 255), -1);
//               //	shapes[0].part(i).x();//68个
//           }
//       }

//       cv::imshow("1", face);
//       cv::waitKey(1000/rate);
//    }
//    cap.release();
////   cv::waitKey(0);

//}

