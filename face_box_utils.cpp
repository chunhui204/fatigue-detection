#include "face_box_utils.hpp"

void face_box_adjustment(std::vector<face_box>& boxes, std::vector<face_box>& boxes_adjust, float img_h, float img_w)
{
    for(auto box : boxes)
    {
        face_box new_box;
        new_box.x0 = box.x0;
        new_box.x1 = box.x1;
        new_box.y0 = box.y0;
        new_box.y1 = box.y1;

        float h = box.y1 - box.y0 + 1;
        float w = box.x1 - box.x0 + 1;
        if(h > w)
        {
            new_box.y0 += (h-w)/2;
            new_box.y1 += (h-w)/2;
            new_box.y0 = std::max(1.f, new_box.y0 - h/5);
            new_box.y1 = std::min(img_h-1, new_box.y1 + h/5);
            new_box.x0 = std::max(1.f, new_box.x0 -(h-w)/2- h/5);
            new_box.x1 = std::min(img_w-1, new_box.x1 + (h-w)/2 + h/5);
        }
        boxes_adjust.push_back(new_box);
    }
}
