#ifndef TENSORFLOWMTCNN_H
#define TENSORFLOWMTCNN_H
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <assert.h>
#include <stdio.h>

#include "tensorflow/cc/ops/const_op.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/stringpiece.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"


#include <opencv2/opencv.hpp>
#include <cv.h>
#include "common_lib.hpp"
#include "utils.hpp"
#include "face_box_utils.hpp"

bool load_graph(const std::string& pb_fname, tensorflow::GraphDef &graphdef, tensorflow::Session *& session, int gpu_id);
void mtcnn_detect(tensorflow::Session * &sess, tensorflow::GraphDef& graph, cv::Mat& img,std::vector<face_box>& face_list);

#endif // TENSORFLOWMTCNN_H
