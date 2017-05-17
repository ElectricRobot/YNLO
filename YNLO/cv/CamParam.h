#ifndef CAMPARAM_H
#define CAMPARAM_H
#include <opencv2/opencv.hpp>

namespace ynlo {

class CamParam {
public:
    bool is_fish_eye_;
    cv::Mat K_, D_;
    cv::Mat R_, t_;
};

}

#endif // CAMPARAM_H
