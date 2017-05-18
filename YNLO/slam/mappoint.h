#ifndef MAPPOINT_H
#define MAPPOINT_H
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>
#include "keyframe.h"

namespace ynlo {

class MapPoint {
public:
    /* global position */
    cv::Mat x3Dw_;
    /* the keyframes have seen this mappoint */
    /* and the index in 2D feature in keyfrme*/
    std::map<std::weak_ptr<KeyFrame>, size_t> observations_;
};

}

#endif // MAPPOINT_H
