/*
 * Camera.h
 *
 *  Created on: Jun 1, 2017
 *      Author: liu
 */

#ifndef CAMERA_H
#define CAMERA_H
#include <memory>
#include <opencv2/core.hpp>
#include "util.h"

namespace ynlo {

class Camera {
public:
    SMART_PTR(Camera)

    /* Constructor */
    Camera() {}
    virtual ~Camera() {}

    /* Assignment */

    /* getter */
    const cv::Size& ImageSize() const;
    const cv::Mat& K() const;
    const cv::Mat& D() const;
    /* setter */
    void SetImageSize(const cv::Size& image_size);
    void SetCameraModel(const cv::Mat& K, const cv::Mat& D);

    /* Method */
    virtual void UndistortImage(cv::InputArray distorted, cv::OutputArray undistorted) = 0;
    virtual void UndistortPoints(cv::InputArray distorted, cv::OutputArray undistorted) = 0;

private:
    virtual void InitUndistortMap() = 0;

    cv::Size image_size_;
    cv::Mat K_, D_;
    cv::Mat undistort_map1_, undistort_map2_;
};

class PinholeCamera : public Camera
{
public:

};

class FisheyeCamera : public Camera {
public:

};
}

#endif /* CAMERA_H */
