#ifndef FACE_BOX_UTILS_HPP
#define FACE_BOX_UTILS_HPP

#include "common_lib.hpp"
#include <vector>


void face_box_adjustment(std::vector<face_box>& boxes, std::vector<face_box>& boxes_adjust, float h, float w);
#endif // FACE_BOX_UTILS_HPP
