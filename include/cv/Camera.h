/*
 * Camera.h
 *
 *  Created on: Jun 1, 2017
 *      Author: liu
 */

#ifndef CAMERA_H
#define CAMERA_H
#include <array>
#include <memory>
#include <opencv2/core.hpp>
#include <Eigen/Core>
#include "util.h"

namespace ynlo {

// this class copy from vikit library made by uzh-rpg
class AbstractCamera {
public:
    SMART_PTR(AbstractCamera)

    AbstractCamera();
    AbstractCamera(int width, int height);
    virtual ~AbstractCamera();

    // Project from pixels to world coordiantes. Returns a bearing vector of unit length.
    virtual Eigen::Vector3d Cam2World(const double& x, const double& y) const = 0;
    virtual Eigen::Vector3d Cam2World(const Eigen::Vector2d& px) const = 0;

    virtual Eigen::Vector2d World2Cam(const Eigen::Vector3d& x3Dc) const = 0;
    // project unit plane coordinates to camera coordinates
    virtual Eigen::Vector2d World2Cam(const Eigen::Vector2d& uv) const = 0;

    // unknow but copy...
    virtual double ErrorMultiplier2() const = 0;
    virtual double ErrorMultiplier() const = 0;

    int Width() const;
    int Height() const;

    bool IsInFrame(const Eigen::Vector2i& obs, int boundary = 0) const;
    bool IsInFrame(const Eigen::Vector2i& obs, int boundary, int level) const;
protected:
    int width_;
    int height_;
};


class PinholeCamera : public AbstractCamera {
public:
    SMART_PTR(PinholeCamera)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    // TODO
private:
    double fx_, fy_;
    double cx_, cy_;
    bool distortion_;
    std::array<double,5> d_;
    cv::Mat cvK_, cvD_;
    cv::Mat undist_map1_, undist_map2_;
    Eigen::Matrix3d K_;
    Eigen::Matrix3d K_inv_;
};

}

#endif /* CAMERA_H */
