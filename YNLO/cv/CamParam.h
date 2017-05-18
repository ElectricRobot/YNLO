#ifndef CAMPARAM_H
#define CAMPARAM_H
#include <opencv2/opencv.hpp>

namespace ynlo {

class CamParam {
public:
    bool is_fish_eye_;
    cv::Size image_size_;
    cv::Mat K_, D_;
};

}

#endif // CAMPARAM_H
