#ifndef FRAME_H
#define FRAME_H
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>
#include "cv/CamParam.h"
#include "cv/SparseFeature.h"
#include "mappoint.h"

namespace ynlo {

class Frame : public CamParam, public SparseFeature
{
public:
    /* 2D 3D match pair */
    std::vector<std::shared_ptr<MapPoint> > matches_mappoints_;
    /* camera pose */
    cv::Mat Tcw_;
};

class ORBFrame : public Frame {

};

}

#endif // FRAME_H
